#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>

#include "pretty_printing.h"

using namespace std;

typedef uint8_t Square;

inline Square make_empty() {
  return 0;
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
  return !s;
}

inline bool is_classic(Square s) {
  return (s & 0xf0) == 0xf0;
}

inline int get_age(Square s) {
  return s & 0xf;
}

inline bool is_x(Square s) {
  return get_age(s) % 2 == 0;
}

inline int get_link(Square s) {
  return s >> 4;
}


class Position {
public:
  uint8_t field[9];

  Position() {
    for (int i = 0; i < sizeof field; i++)
      field[i] = make_empty();
  }

  template<typename stream>
  void print(stream &out) {
    const string big_x = "**  **  **  **  **";
    const string big_o = " **** **  ** **** ";
    string square_contents[9];
    for (int i = 0; i < 9; i++) {
      Square s = field[i];
      if (::is_empty(s)) continue;
      if (is_classic(s)) {
        square_contents[i] = is_x(s) ? big_x : big_o;
        square_contents[i][17] = '1' + get_age(i);
      }
    }
    for (int age = 0; age < 9; age++) {
      for (int i = 0; i < 9; i++) {
        Square s = field[i];
        if (::is_empty(s)) continue;
        if (is_classic(s)) continue;
        if (get_age(s) != age) continue;
        string mark = (is_x(s) ? "x" : "o") + string(1, '1' + age);
        square_contents[i] += mark;
        square_contents[get_link(s)] += mark;
      }
    }

    for (int i = 0; i < 3; i++) {
      if (i > 0)
        out << "------+------+------" << endl;
      for (int ii = 0; ii < 3; ii++) {
        for (int j = 0; j < 3; j++) {
          string s = square_contents[3*i+j];
          s += string(18 - s.size(), ' ');
          if (j > 0)
            out << "|";
          out << s.substr(6 * ii, 6);
        }
        out << endl;
      }
    }
  }
};


int main() {
  Position p;
  p.field[0] = make_classic(0);
  p.field[1] = make_classic(1);
  p.field[2] = make_quantum(3, 3);
  p.field[3] = make_quantum(2, 8);
  p.print(cout);
  cout << "hello, world" << endl;
  return 0;
}
