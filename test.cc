#include <iostream>
#include <string>
#include <vector>

#include "position.h"

using namespace std;


void run_sequence(vector<string> move_names) {
  Position position;

  for (auto move_name : move_names) {
    if (move_name == "show") {
      cout << endl;
      position.print(cout);
      continue;
    }
    cout << move_name << " ";
    Move moves[36];
    auto moves_end = moves;

    position.enumerate_moves(moves_end);
    assert(moves_end - moves <= 36);

    auto ptr = moves;
    for (; ptr != moves_end; ptr++) {
      if (ptr->name == move_name) {
        position = ptr->position;
        break;
      }
    }
    assert(ptr != moves_end);
  }
  cout << endl;
  position.print(cout);
  cout << endl;
}


int main() {
  run_sequence({"12", "34"});
  run_sequence({"12", "12?" , "1!"});
  run_sequence({"12", "23", "34", "45", "15?", "show", "5!"});
  run_sequence({"12", "23", "34", "45", "15?", "1!"});
  run_sequence({"45", "34", "23", "12", "15?", "show", "1!"});
  run_sequence({"14", "34", "45", "49", "59?", "show", "9!"});
  run_sequence({"12", "12?", "1!", "45", "45?", "4!", "78", "78?", "7!"});
  run_sequence({"13", "35", "89", "57", "12", "27?", "show", "7!"});
  run_sequence({"12", "23", "35", "45", "47", "67", "69", "89", "18?", "show", "8!"});
  return 0;
}
