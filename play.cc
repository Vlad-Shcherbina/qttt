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

  auto weak_win = [](const Position &p) {return p.x_score == 2;};
  auto strong_win = [](const Position &p) {return p.x_score && p.o_score == 0;};

  Position position;
  while (!position.final) {
    position.print(cout);

    vector<Move> moves;
    position.enumerate_moves(back_inserter(moves));
    assert(0 < moves.size() && moves.size() <= 36);
    if (position.is_x_move()) {
      //printf("I have following weakly winning moves: ");
      function<bool(const Position &)> win = weak_win;
      if (position.age > 0 && x_disj_o_conj(position, strong_win)) {
        printf("now you are doomed\n");
        win = strong_win;
      }
      bool move_made = false;
      for (auto move : moves)
        if (x_disj_o_conj(move.position, win)) {
          printf("my move is %s\n", move.name);
          position = move.position;
          move_made = true;
          break;
        }
      assert(move_made);
    } else {
      printf("pick your move (");
      bool first = true;
      for (auto move : moves) {
        if (!first) printf(" ");
        first = false;
        printf("%s", move.name);
      }
      printf("):\n");
      char buf[80];
      while (true) {
        printf("> ");
        fgets(buf, sizeof buf, stdin);
        string m = string(buf);
        assert(m.back() == '\n');
        m.erase(m.length()-1);
        bool move_made = false;
        for (auto move : moves)
          if (m == move.name) {
            position = move.position;
            move_made = true;
            break;
          }
        if (move_made) break;
        printf("not a valid move\n");
      }
    }
  }
  position.print(cout);
}
