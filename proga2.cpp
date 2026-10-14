#include <iostream>
#include <random>
#include <string>

// random number generator is from AI tips
static std::mt19937 rng{std::random_device{}()};
static std::bernoulli_distribution coin{0.5}; // 50/50

void op_deposit(int &sum) {
  int k;
  std::cin >> k;
  if (k <= 0) {
    std::cout << "error\n";
    return;
  }
  sum += k;
}

void op_withdraw(int &sum) {
  int k;
  std::cin >> k;
  if (k <= 0 || k > sum) {
    std::cout << "error\n";
    return;
  }
  sum -= k;
}

void op_balance(int &sum) { cout << sum << '\n'; }

void op_reset(int &sum) { sum = 0; }

void op_casino(int &sum) {
  int bet;
  cin >> bet;
  if (bet <= 0 || bet > sum) {
    std::cout << "error\n";
    return;
  }

  if (coin(rng)) {
    sum += bet;
    cout << "win\n";
  } else {
    sum -= bet;
    cout << "lose\n";
  }
}

int main() {
  const std::string names[] = {"deposit", "withdraw", "balance",
                               "reset",   "inc",      "casino"};
  void *(int &)funcs[] = {op_deposit, op_withdraw, op_balance,
                          op_reset,   op_inc,      op_casino};
  const int N = 6;

  int sum = 0;
  string cmd;
  while (cin >> cmd) {
    if (cmd == "stop")
      break;
    bool found = false;
    for (int i = 0; i < N; ++i) {
      if (names[i] == cmd) {
        funcs[i](sum);
        found = true;
        break;
      }
    }
    if (!found)
      std::cout << "error\n";
  }
}
