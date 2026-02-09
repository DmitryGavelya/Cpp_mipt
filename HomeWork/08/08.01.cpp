#include <iostream>

class Entity_v1 {
private:
  int value_;

public:
  Entity_v1(int v = 0) : value_(v) {}

  void print() const {
    std::cout << "Entity_v1::value_ = " << value_ << '\n';
  }
};

class Entity_v2 {
public:
  int value;

  Entity_v2(int v = 0) : value(v) {}
};

int main() {
  Entity_v1 e1(10);
  e1.print();

  Entity_v2 &hack = reinterpret_cast<Entity_v2&>(e1);
  hack.value = 42;

  e1.print();

  return 0;
}
