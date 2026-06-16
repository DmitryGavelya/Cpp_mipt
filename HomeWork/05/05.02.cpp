#include <iostream>

class Entity {
public:
  virtual ~Entity() = default;
  virtual void test() const = 0;
};

class Client : public Entity {
public:
  void test() const override { std::cout << "Client::test\n"; }
};

class LoggingBehavior {
public:
  static void additionalBehavior() {
    std::cout << "LoggingBehavior::additionalBehavior\n";
  }
};

template <typename T> class Decorator : public Entity {
private:
  Entity *m_entity;
  T m_behavior;

public:
  explicit Decorator(Entity *entity) : m_entity(entity) {}

  void test() const override {
    m_entity->test();
    m_behavior.additionalBehavior();
  }
};

int main() {
  Entity *client = new Client();
  Entity *decorated = new Decorator<LoggingBehavior>(client);

  decorated->test();

  delete decorated;
  delete client;
}