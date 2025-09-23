// Реализуйте алгоритм вычисления наибольшей длины последовательности Коллатца среди всех последова-
//   тельностей, начинающихся со значений от 1 до 100. Используйте тип unsigned long long int для значений
//   последовательностей Коллатца и псевдоним std::size_t для их длин. Используйте кэширование длин по-
//   следовательностей Коллатца в контейнере std::vector для оптимизации вычисления длины каждой новой
//   последовательностинаосновепредыдущихпоследовательностей.Используйтестандартныйсимвольныйпоток
//   вывода std::cout для вывода наибольшей длины последовательности Коллатца среди всех рассмотренных,
//   а также соответствующего ей начального значения. Не сопровождайте Ваше решение данной задачи тестами.

#include <iostream>

static unsigned long long collatz(unsigned long long n, long long length, std::vector<unsigned long long> &cache) {
  if (cache[n] != 0) return cache[n] + length;
  if (n % 2 == 0) return collatz(n / 2, length + 1, cache);
  if (n % 2 != 0) return collatz(3 * n + 1, length + 1, cache);

}

int main() {
  std::vector<unsigned long long> cache(100);
  cache.at(0) = 1;
  cache.at(1) = 1;
  unsigned long long max = 0;
  for (std::size_t i = 2; i < cache.size(); ++i) {
    unsigned long long res = collatz(i, 0, cache);
    cache.at(i) = res - 1;
    max = std::max(max, cache[i]);
  }
  std::cout << max << '\n';
}
