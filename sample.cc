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
  p.field[3] = make_quantum(2, 8);
  p.print(cout);
  cout << "hello, world" << endl;
  return 0;
}
