#include <iostream>

struct Wrapper;

Wrapper test();

struct Wrapper {
  using FuncPtr = Wrapper (*)();
  FuncPtr ptr;

  Wrapper(FuncPtr p) : ptr(p) {}

  operator FuncPtr() const {
    return ptr;
  }
};

Wrapper test() {
  std::cout << "test() called\n";
  return Wrapper(test);
}

int main() {
  Wrapper function = test();
  (*function)();

}
