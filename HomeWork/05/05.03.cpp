#include <iostream>

/////////////////////////////////////////////////////////

class Strategy {
public:
  virtual ~Strategy() = default;
  virtual void test() const = 0;
};

/////////////////////////////////////////////////////////

class Slow : public Strategy {
public:
  void test() const override { std::cout << "Slow::test\n"; }
};

/////////////////////////////////////////////////////////

class Fast : public Strategy {
public:
  void test() const override { std::cout << "Fast::test\n"; }
};

/////////////////////////////////////////////////////////

template <typename T> class Entity {
public:
  Entity(T strategy = T{}) : m_strategy(strategy) {}

  void test() const { m_strategy.test(); }

private:
  T m_strategy;
};

/////////////////////////////////////////////////////////

int main() {
  Entity<Slow> slowEntity;
  Entity<Fast> fastEntity;

  slowEntity.test();
  fastEntity.test();

  Slow slowStrategy;
  Entity<Slow> entityWithStrategy(slowStrategy);
  entityWithStrategy.test();
}

/////////////////////////////////////////////////////////