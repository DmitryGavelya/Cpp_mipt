#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

template <typename RandomIt, typename Compare>
void order(RandomIt first, RandomIt last, Compare comp) {
  if (std::distance(first, last) <= 1)
    return;

  for (auto i = std::next(first); i != last; ++i) {
    for (auto j = i; j != first && comp(*j, *std::prev(j)); --j) {
      std::iter_swap(j, std::prev(j));
    }
  }
}

template <typename RandomIt, typename Compare>
RandomIt partition(RandomIt first, RandomIt last, Compare comp) {
  auto pivot_it = std::prev(last);
  auto pivot = *pivot_it;
  auto i = first;

  for (auto j = first; j != pivot_it; ++j) {
    if (!comp(pivot, *j)) {
      std::iter_swap(i, j);
      std::advance(i, 1);
    }
  }
  std::iter_swap(i, pivot_it);
  return i;
}

template <typename RandomIt, typename Compare>
void quickSort(RandomIt first, RandomIt last, Compare comp) {
  auto dist = std::distance(first, last);
  if (dist <= 1)
    return;

  if (dist > 16) {
    auto pi = partition(first, last, comp);
    quickSort(first, pi, comp);
    quickSort(std::next(pi), last, comp);
  } else {
    order(first, last, comp);
  }
}

template <typename RandomIt, typename Compare>
void sort(RandomIt first, RandomIt last, Compare comp) {
  quickSort(first, last, comp);
}

bool custom_greater(int a, int b) { return a > b; }

int main() {
  auto size = 1000;

  std::vector<int> v1(size);
  std::iota(v1.begin(), v1.end(), 1);
  ::sort(v1.begin(), v1.end(), custom_greater);
  assert(std::is_sorted(v1.begin(), v1.end(), custom_greater));

  std::vector<int> v2(size);
  for (auto i = 0; i < size; ++i)
    v2[i] = size - i;
  ::sort(v2.begin(), v2.end(), std::less<int>{});
  assert(std::is_sorted(v2.begin(), v2.end(), std::less<int>{}));

  std::vector<int> v3 = {0, -5, 2, -9, 4, 1, -1};
  auto lambda_comp = [](int a, int b) { return std::abs(a) < std::abs(b); };
  ::sort(v3.begin(), v3.end(), lambda_comp);
  assert(std::is_sorted(v3.begin(), v3.end(), lambda_comp));
}
