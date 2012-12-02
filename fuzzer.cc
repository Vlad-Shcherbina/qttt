#include <iostream>
#include <cstdlib>
#include <ctime>

#include "position.h"

using namespace std;


int main() {
  int num_moves = 0;
  double start = clock();
  while (true) {
    Position position;
    while (!position.final) {
      Move moves[36];
      auto moves_end = moves;

      position.enumerate_moves(moves_end);

      assert(moves_end - moves <= 36);
      assert(moves_end > moves);

      auto &move = moves[rand() % (moves_end - moves)];
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
