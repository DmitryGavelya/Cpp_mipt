#include <iostream>
#include <memory>

class Strategy {
public:
  virtual ~Strategy() = default;
  virtual void test() const = 0;
};

class Client : public Strategy {
public:
  void test() const override { std::cout << "Client::test\n"; }
};

class Entity {
public:
  Entity(std::unique_ptr<Strategy> strategy) : m_strategy(std::move(strategy)) {}

  void test() const {
    if (m_strategy) m_strategy->test();
  }

private:
  std::unique_ptr<Strategy> m_strategy;
};

int main() {
  Entity entity(std::make_unique<Client>());

  entity.test();
}
