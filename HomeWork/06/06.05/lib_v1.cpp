

#include <iostream>

extern "C" {
  void test_v1() {
    std::cout << "Library 1 - test_v1 called\n";
  }

  void test_v2() {
    std::cout << "Library 1 - test_v2 called\n";
  }

  void test_v3() {
    std::cout << "Library 1 - test_v3 called\n";
  }
}
