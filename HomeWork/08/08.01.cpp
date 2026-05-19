#include <iostream>

class Entity_v1 {
private:
  int value_;
public:
  Entity_v1(int v = 0) : value_(v) {}
  void print() const { std::cout << value_ << '\n'; }
};

class Entity_v2 {
public:
  int value;
  Entity_v2(int v = 0) : value(v) {}
};

struct EntitySecretTag {
  using type = int Entity_v1::*;
};

template <typename Tag>
struct Steal {
  static typename Tag::type ptr;
};

template <typename Tag>
typename Tag::type Steal<Tag>::ptr;

template <typename Tag, typename Tag::type Ptr>
struct Stealer {
  struct Filler {
    Filler() { Steal<Tag>::ptr = Ptr; }
  };
  static Filler filler;
};

template <typename Tag, typename Tag::type Ptr>
typename Stealer<Tag, Ptr>::Filler Stealer<Tag, Ptr>::filler;

template struct Stealer<EntitySecretTag, &Entity_v1::value_>;

int main() {
  Entity_v1 e1(10);
  e1.print();

  Entity_v2& hack_ref = reinterpret_cast<Entity_v2&>(e1);
  hack_ref.value = 42;
  e1.print();

  int* hack_ptr = reinterpret_cast<int*>(&e1);
  *hack_ptr = 99;
  e1.print();

  e1.*Steal<EntitySecretTag>::ptr = 777;
  e1.print();

  return 0;
}
