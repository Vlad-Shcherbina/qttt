#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <functional>

#include "pretty_printing.h"
#include "position.h"

using namespace std;

template<typename F>
bool x_disj_o_conj(const Position &position, F leaf_eval) {
  if (position.final)
    return leaf_eval(position);

  vector<Move> moves;
  moves.reserve(36);
  position.enumerate_moves(back_inserter(moves));
  assert(moves.size() <= 36);

  if (position.is_x_move()) {
    for (auto &move : moves)
      if (x_disj_o_conj(move.position, leaf_eval))
        return true;
    return false;
  } else {
    for (auto &move : moves)
      if (!x_disj_o_conj(move.position, leaf_eval))
        return false;
      return true;
  }
}


int main() {
  Position p;
  p.print(cout);

  bool result = x_disj_o_conj(
      p,
      [](const Position &p) {return p.x_score == 2 && p.o_score == 0;});
  if (result)
    printf("x wins\n");
  else
    printf("x does not win\n");

  return 0;

  vector<Move> moves;

  p.enumerate_moves(back_inserter(moves));
  assert(moves.size() <= 36);

  cout << "==========" << endl;
  for (auto move : moves) {
    cout << move.name << endl;
    move.position.print(cout);
  }

  cout << "hello, world" << endl;
  return 0;
}
