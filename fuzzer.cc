#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <vector>

#include "position.h"

using namespace std;


int main() {
  int num_moves = 0;
  double start = clock();
  while (true) {
    Position position;
    while (!position.final) {
      vector<Move> moves;
      moves.reserve(36);

      position.enumerate_moves(back_inserter(moves));

      assert(!moves.empty());
      assert(moves.size() <= 36);

      auto &move = moves[rand() % moves.size()];
      position = move.position;
      num_moves += 1;

    }
    if (num_moves > 0 && num_moves % 100000 == 0) {
      double t = (clock() - start) /  CLOCKS_PER_SEC;
      cout << num_moves / t << " moves per second" << endl;
    }
  }
  return 0;
}
