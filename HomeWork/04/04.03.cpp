

#include <cassert>
#include <iostream>
#include <vector>


template <typename Container> void add_to_container(Container& container, auto... args) {
  (container.push_back(args), ...);
}

int main() {
  std::vector<int> v1;
  add_to_container(v1, 1, 2, 3);
  std::vector<int> expected1 = {1, 2, 3};
  assert(v1 == expected1);

  std::vector<int> v2;
  add_to_container(v2, 42);
  assert(v2.size() == 1);
  assert(v2[0] == 42);

  std::vector<int> v3;
  add_to_container(v3);
  assert(v3.empty());

  std::vector<int> v4;
  add_to_container(v4, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  assert(v4.size() == 10);
  std::vector<int> expected4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  assert(v4 == expected4);

  std::vector<int> v5;
  add_to_container(v5, -1, -2, -3);
  std::vector<int> expected5 = {-1, -2, -3};
  assert(v5 == expected5);

  std::vector<int> v6;
  add_to_container(v6, 0, 0, 0);
  std::vector<int> expected6 = {0, 0, 0};
  assert(v6 == expected6);
}