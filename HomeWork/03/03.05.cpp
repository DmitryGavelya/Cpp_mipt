// Реализуйте систему раздельного переопределения наследуемых виртуальных
// функций-членов с одинаковы-ми сигнатурами. Реализуйте базовые классы
// Entity_v1 и Entity_v2. Реализуйте
//   в классах Entity_v1 и Entity_v2 виртуальные функции-члены test, которые
//   решают разные задачи, но имеют одинаковые сигнату- ры. Реализуйте
//   производный класс Client, который является наследником интерфейсов классов
//   Entity_v1 и Entity_v2. Обратите внимание, что раздельное переопределение
//   наследуемых виртуальных функций-членов test в классе Client невозможно
//   из-за совпадения сигнатур. Реализуйте дополнительные классы Adapter_v1 и
//   Adapter_v2 в иерархии наследования между классами Entity_v1 и Client и
//   между классами Entity_v2 и Client соответственно. Реализуйте виртуальные
//   функции-члены test_v1 и test_v2 в классах Adapter_v1 и Adapter_v2
//   соответственно, которые будут выступать в роли посредников с разными
//   сигнатурами и позво- лят выполнить раздельное переопределение наследуемых
//   виртуальных функций-членов test в классе Client.

#include <iostream>

class Entity_v1 {
public:
  virtual ~Entity_v1() = default;
  virtual void test() { std::cout << "test from Entity_v1" << '\n'; }
};

class Entity_v2 {
public:
  virtual ~Entity_v2() = default;
  virtual void test() { std::cout << "test from Entity_v2" << '\n'; }
};

class Adapter_v1 : public Entity_v1 {
  public:
  virtual void test_v1() {}

  private:
  void test() final override {
    test_v1();
  }
};

class Adapter_v2 : public Entity_v2{
  public:
    virtual void test_v2() {}

  private:
    void test() final override {
      test_v2();
    }
};

class Client : public Adapter_v1, public Adapter_v2 {
  public:
    void test_v1() override {
      std::cout << "test_v1";
    }

  void test_v2() override {
      std::cout << "test_v2";
    }
};

int main() {}