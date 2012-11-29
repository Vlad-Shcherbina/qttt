#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

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


class Position {
public:
  uint8_t field[9];

  Position() {
    for (int i = 0; i < sizeof field; i++)
      field[i] = make_empty();
  }

  template<typename stream>
  void print(stream &out) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        Square s = field[i*3+j];
        if (::is_empty(s))
          out << ".";
        else if (is_classic(s))
          out << (is_x(s) ? "x" : "o");
        else
          out << "?";
        out << " ";
      }
      out << endl;
    }
  }
};


int main() {
  Position p;
  p.field[0] = make_classic(0);
  p.field[1] = make_classic(1);
  p.field[2] = make_quantum(2, 3);
  p.print(cout);
  cout << "hello, world" << endl;
  return 0;
}
