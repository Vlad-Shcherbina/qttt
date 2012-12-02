#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>

#include "pretty_printing.h"
#include "position.h"

using namespace std;


int main() {
  Position p;
  p.field[0] = make_classic(0);
  p.field[1] = make_classic(1);
  p.field[2] = make_quantum(3, 3);
  p.field[3] = make_quantum(2, 6);
  p.age = 4;

  p.collapsing = true;
  p.collapse1 = 2;
  p.collapse2 = 3;

  p.print(cout);

  Move moves[36];
  auto end = moves;

  p.enumerate_moves(end);
  assert(end - moves <= 36);

  cout << "==========" << endl;
  for (auto ptr = moves; ptr != end; ptr++) {
    cout << ptr->name << endl;
    (ptr->position).print(cout);
  }

  cout << "hello, world" << endl;
  return 0;
}
