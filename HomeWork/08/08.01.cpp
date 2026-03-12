#include <iostream>
#include <cassert>
#include <type_traits>

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
  static_assert(sizeof(Entity_v1) == sizeof(Entity_v2), "Size mismatch between Entity_v1 and Entity_v2");
  static_assert(alignof(Entity_v1) == alignof(Entity_v2), "Alignment mismatch between Entity_v1 and Entity_v2");
  static_assert(std::is_standard_layout_v<Entity_v1>, "Entity_v1 is not standard layout");
  static_assert(std::is_standard_layout_v<Entity_v2>, "Entity_v2 is not standard layout");

  Entity_v1 e1(10);
  Entity_v2 &hack1 = reinterpret_cast<Entity_v2&>(e1);

  assert(hack1.value == 10);

  hack1.value = 42;
  assert(hack1.value == 42);

  Entity_v1 e2(100);
  Entity_v2 &hack2 = reinterpret_cast<Entity_v2&>(e2);

  assert(hack2.value == 100);
  hack2.value = 999;
  assert(hack2.value == 999);

  e1.print();
  e2.print();

  return 0;
}
