#include <cassert>
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


class Position {
public:
  Square field[9];
  int age;

  Position() {
    for (int i = 0; i < 9; i++)
      field[i] = make_empty();
    age = 0;
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
      std::cout << a << std::endl;
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

    return true;
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
  }
};
