#include <iostream>
#include <string>
using namespace std;

const int ROWS = 5;
const int COLS = 10;
char board[ROWS][COLS];

void clear() {
  for (int y = 0; y < ROWS; ++y)
    for (int x = 0; x < COLS; ++x)
      board[y][x] = ' ';
}

bool put(int x, int y, char c) {
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS)
    return false;
  board[y][x] = c;
  return true;
}

bool text(int x, int y, const string& word) {
  int len = (int)word.size();
  if (y < 0 || y >= ROWS)
    return false;
  if (x < 0 || x + len > COLS)
    return false;
  for (int i = 0; i < len; ++i)
    board[y][x + i] = word[i];
  return true;
}

void print() {
  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x)
      cout << board[y][x];
    cout << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  clear();
  string command;
  while (cin >> command) {
    if (command == "quit") {
      break;
    } else if (command == "clear") {
      clear();
    } else if (command == "put") {
      int x, y;
      char c;
      cin >> x >> y >> c;
      if (!put(x, y, c))
        cout << "error\n";
    } else if (command == "text") {
      int x, y;
      string word;
      cin >> x >> y >> word;
      if (!text(x, y, word))
        cout << "error\n";
    } else if (command == "print") {
      print();
    }
  }
  return 0;
}
