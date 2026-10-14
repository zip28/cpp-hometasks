#include <iomanip>
#include <iostream>

using namespace std;

int FindMin(const int n, const int t[]) {
  int ans = t[0];
  for (int i = 0; i < n; ++i) {
    if (t[i] < ans)
      ans = t[i];
  }
  return ans;
}

int FindMax(const int n, const int t[]) {
  int ans = t[0];
  for (int i = 0; i < n; ++i) {
    if (t[i] > ans)
      ans = t[i];
  }
  return ans;
}

double FindMedium(const int n, const int t[]) {
  double summ = 0;
  for (int i = 0; i < n; ++i) {
    summ += t[i];
  }
  return summ / n;
}

int CountColdDays(const int n, const int t[]) {
  int ans = 0;
  for (int i = 0; i < n; ++i) {
    if (t[i] < 0)
      ++ans;
  }
  return ans;
}

int main() {
  int n;
  cin >> n;
  int t[n];
  for (int i = 0; i < n; ++i) {
    cin >> t[i];
  }
  cout << fixed << setprecision(1) << FindMedium(n, t) << '\n';
  cout << FindMin(n, t) << ' ' << FindMax(n, t) << '\n';
  cout << CountColdDays(n, t) << '\n';
}
