// Реализуйте алгоритм вычисления наибольшего общего делителя двух натуральных чисел типа int на основе
// рекурсивного подхода, а также алгоритм вычисления наименьшего общего кратного двух натуральных чисел
 // типа int. Используйте
// стандартные функции std::gcd и std::lcm для валидации результатов
// тестирования.

#include <assert.h>
#include <iostream>
#include <numeric>

int gcd(int a, int b) {
  if (b == 0) return a;
  return gcd(b, a % b);
}

int lcm(int a, int b) {
  return a /gcd(a, b) * b;
}

int main() {
  assert(gcd(15, 25) == std::gcd(15, 25));
  assert(gcd(48, 18) == std::gcd(48, 18));
  assert(gcd(17, 13) == std::gcd(17, 13));
  assert(gcd(100, 10) == std::gcd(100, 10));
  assert(gcd(7, 7) == std::gcd(7, 7));
  assert(lcm(15, 25) == std::lcm(15, 25));
  assert(lcm(48, 18) == std::lcm(48, 18));
  assert(lcm(17, 13) == std::lcm(17, 13));
  assert(lcm(100, 10) == std::lcm(100, 10));
  assert(lcm(7, 7) == std::lcm(7, 7));
  assert(gcd(1, 100) == 1);
  assert(lcm(1, 100) == 100);
  assert(gcd(15, 25) == std::gcd(15, 25));
}