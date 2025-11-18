#include <iostream>

consteval double compute_e(double epsilon) {
  double e = 1.0;
  double term = 1.0;
  int n = 1;

  while (term >= epsilon) {
    term /= n;
    e += term;
    ++n;
  }
  return e;
}

constexpr std::array<double, 5> epsilons = {1e-1, 1e-2, 1e-3, 1e-4, 1e-5};

int main() {
  static_assert(compute_e(epsilons.at(0)) > 2.5 && compute_e(epsilons.at(0)) < 3.0);
  static_assert(compute_e(epsilons.at(1)) > 2.7 && compute_e(epsilons.at(1)) < 2.8);
  static_assert(compute_e(epsilons.at(2)) > 2.718 && compute_e(epsilons.at(2)) < 2.719);
}