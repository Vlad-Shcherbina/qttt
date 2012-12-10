#include <cassert>
#include <cstring>
#include <string>
#include <iostream>


typedef uint8_t Square;


inline Square make_empty() {
  return 0xff;
}

inline Square make_classic(int age) {
  assert(age >= 0 && age < 9);
  return 0xf0 | age;
}

inline Square make_quantum(int age, int link) {
  assert(age >= 0 && age < 9);
  assert(link >= 0 && link < 9);
  return (link << 4) + age;
}

inline bool is_empty(Square s) {
  return s == 0xff;
}

inline bool is_classic(Square s) {
  assert(!is_empty(s));
  return (s & 0xf0) == 0xf0;
}

inline int get_age(Square s) {
  assert(!is_empty(s));
  return s & 0xf;
}

inline bool is_x(Square s) {
  return get_age(s) % 2 == 0;
}

inline int get_link(Square s) {
  assert(!is_empty(s) && !is_classic(s));
  return s >> 4;
}

struct Move;

class Position {
public:
  Square field[9];
  int age;

  bool collapsing;
  int collapse1, collapse2;

  bool final;
  int x_score, o_score;

  Position() {
    for (int i = 0; i < 9; i++)
      field[i] = make_empty();
    age = 0;
    collapsing = false;
    final = false;
  }

  bool is_sane() const {
    // We use asserts instead of return false for better diagnostics.
    assert(0 <= age && age <= 9);

    for (int i = 0; i < 9; i++) {
      Square s = field[i];
      if (is_empty(s)) continue;
      int age = get_age(s);
      assert(0 <= age < 9);
      if (is_classic(s)) continue;
      int link = get_link(s);
      assert(0 <= link && link < 9);
      assert(link != i);
      int linked = field[link];
      assert(is_empty(linked) || !is_classic(linked));
    }

    for (int a = 0; a < 9; a++) {
      bool found = false;
      for (int i = 0; i < 9; i++) {
        Square s = field[i];
        if (is_empty(s))
          continue;
        if (get_age(s) == a) {
          assert(!found);
          found = true;
        }
      }
      if (a < age)
        assert(found);
      else
        assert(!found);
    }

    if (collapsing) {
      assert(age < 9);
      assert(0 <= collapse1 < 9);
      assert(0 <= collapse2 < 9);
      assert(is_empty(field[collapse1]) || !is_classic(field[collapse1]));
      assert(is_empty(field[collapse2]) || !is_classic(field[collapse2]));
    }

    if (final) {
      assert(age >= 3);
      assert(
          x_score == 2 && (o_score == 0 || o_score == 1) ||
          o_score == 2 && (x_score == 0 || x_score == 1) ||
          x_score == 1 && o_score == 1
      );
    }

    return true;
  }

  bool  is_x_move() const {
    if (!collapsing)
      return age % 2 == 0;
    else
      return age % 2 == 1;
  }

  template<typename stream>
  void print(stream &out) const {
    assert(is_sane());

    const std::string big_x = "**  **  **  **  **";
    const std::string big_o = " **** **  ** **** ";
    std::string square_contents[9];
    for (int i = 0; i < 9; i++) {
      Square s = field[i];
      if (is_empty(s)) continue;
      if (is_classic(s)) {
        square_contents[i] = is_x(s) ? big_x : big_o;
        square_contents[i][17] = '1' + get_age(s);
      }
    }
    for (int age = 0; age < 9; age++) {
      for (int i = 0; i < 9; i++) {
        Square s = field[i];
        if (is_empty(s)) continue;
        if (is_classic(s)) continue;
        if (get_age(s) != age) continue;
        std::string mark = (is_x(s) ? "x" : "o") + std::string(1, '1' + age);
        square_contents[i] += mark;
        square_contents[get_link(s)] += mark;
      }
    }

    if (collapsing) {
      std::string mark = age % 2 == 0 ? "x?" : "o?";
      square_contents[collapse1] += mark;
      square_contents[collapse2] += mark;
    }

    for (int i = 0; i < 3; i++) {
      if (i > 0)
        out << "------+------+------" << std::endl;
      for (int ii = 0; ii < 3; ii++) {
        for (int j = 0; j < 3; j++) {
          std::string s = square_contents[3*i+j];
          s += std::string(18 - s.size(), ' ');
          if (j > 0)
            out << "|";
          out << s.substr(6 * ii, 6);
        }
        out << std::endl;
      }
    }
    if (final) {
      out << x_score << ":" << o_score << std::endl;
    }
  }

  void reroot(int pos) {
    Square s = field[pos];
    field[pos] = make_empty();

    while (!is_empty(s)) {
      assert(!is_classic(s));
      int age = get_age(s);
      int link = get_link(s);
      s = field[link];
      field[link] = make_quantum(age, pos);
      pos = link;
    }
  }

  void update_winning_time(
      int start, int step, int &x_winning_time, int &o_winning_time) {
    if (is_empty(field[start]) || !is_classic(field[start]))
      return;
    if (is_empty(field[start + step]) || !is_classic(field[start + step]))
      return;
    if (is_empty(field[start + 2*step]) || !is_classic(field[start + 2*step]))
      return;
    int age1 = get_age(field[start]);
    int age2 = get_age(field[start + step]);
    int age3 = get_age(field[start + 2*step]);
    if (age1 % 2 == age2 % 2 && age1 % 2 == age3 % 2) {
      int t = std::max(age1, std::max(age2, age3));
      int &wt = age1 % 2 == 0 ? x_winning_time : o_winning_time;
      wt = std::min(wt, t);
    }
  }

  void check_ending_conditions() {
    assert(!final);
    int x_winning_time = 100;
    int o_winning_time = 100;
    update_winning_time(0, 1, x_winning_time, o_winning_time);
    update_winning_time(3, 1, x_winning_time, o_winning_time);
    update_winning_time(6, 1, x_winning_time, o_winning_time);
    update_winning_time(0, 3, x_winning_time, o_winning_time);
    update_winning_time(1, 3, x_winning_time, o_winning_time);
    update_winning_time(2, 3, x_winning_time, o_winning_time);
    update_winning_time(0, 4, x_winning_time, o_winning_time);
    update_winning_time(2, 2, x_winning_time, o_winning_time);
    if (x_winning_time < 100)
      if (o_winning_time < 100)
        if (x_winning_time < o_winning_time) {
          final = true; x_score = 2; o_score = 1;
        } else {
          final = true; x_score = 1; o_score = 2;
        }
      else {
        final = true; x_score = 2; o_score = 0;
      }
    else
      if (o_winning_time < 100) {
        final = true; o_score = 2; x_score = 0;
      } else {
        if (age == 9) {
          final = true; x_score = o_score = 1;
        } else if (age == 8) {
          // Special case where all squares but one are occupied with classic
          // pieces, so no move can be made.
          final = true; x_score = o_score = 1;
          for (int i = 0; i < 9; i++)
            if (!is_empty(field[i]) && !is_classic(field[i])) {
              final = false;
              break;
            }
        }
      }
  }

  template<typename OutputIterator>
  void enumerate_moves(OutputIterator out_iter) const {
    assert(!final);
    if (collapsing) {
      int poses[] = {collapse1, collapse2};
      for (int pos : poses) {
        Position new_position(*this);
        new_position.age += 1;
        new_position.collapsing = false;
        new_position.reroot(pos);
        new_position.field[pos] = make_classic(new_position.age-1);
        while (true) {
          bool stabilized = true;
          for (int i = 0; i < 9; i++) {
            Square s = new_position.field[i];
            if (!is_empty(s) && !is_classic(s)) {
              stabilized = false;
              new_position.field[i] = make_classic(get_age(s));
            }
          }
          if (stabilized)
            break;
        }
        new_position.check_ending_conditions();
        char name[] = {char('1' + pos), '!', 0};
        *out_iter++ = Move(name, new_position);
      }
      return;
    }

    int roots[9] = {0};
    for (int i = 0; i < 9; i++) {
      int cur = i;
      if (!is_empty(field[cur]) && is_classic(field[cur])) continue;
      while (!is_empty(field[cur])) {
        assert(!is_classic(field[cur]));
        cur = get_link(field[cur]);
      }
      roots[i] = cur;
    }

    for (int pos1 = 0; pos1 < 9; pos1++) {
      if (!is_empty(field[pos1]) && is_classic(field[pos1])) continue;
      for (int pos2 = pos1+1; pos2 < 9; pos2++) {
        if (!is_empty(field[pos2]) && is_classic(field[pos2])) continue;

        Position new_position(*this);
        if (roots[pos1] == roots[pos2]) {
          new_position.collapsing = true;
          new_position.collapse1 = pos1;
          new_position.collapse2 = pos2;
          char name[] = {char('1' + pos1), char('1' + pos2), '?', 0};
          *out_iter++ = Move(name, new_position);
        } else {
          new_position.age += 1;
          new_position.reroot(pos1);
          assert(is_empty(new_position.field[pos1]));
          new_position.field[pos1] = make_quantum(age, pos2);
          char name[] = {char('1' + pos1), char('1' + pos2), 0};
          *out_iter++ = Move(name, new_position);
        }
      }
    }
  }
};

struct Move {
  char name[4];
  Position position;

  Move() {}
  Move(const char *name, const Position &p) : position(p) {
    std::strcpy(this->name, name);
  }
};
