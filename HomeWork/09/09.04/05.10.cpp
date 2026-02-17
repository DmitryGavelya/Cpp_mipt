#include <iostream>
#include <memory>

class Entity {
public:
  virtual ~Entity() = default;
  virtual void test() const = 0;
};

class Client : public Entity {
public:
  void test() const override { std::cout << "Client::test\n"; }
};

class Decorator : public Entity {
public:
  // Декоратор разделяет владение оборачиваемым объектом
  Decorator(std::shared_ptr<Entity> entity) : m_entity(entity) {}

  void test() const override {
    std::cout << "Decorator::test : ";
    if (m_entity) m_entity->test();
  }

private:
  std::shared_ptr<Entity> m_entity;
};

int main() {
  std::shared_ptr<Entity> entity_1 = std::make_shared<Client>();
  std::shared_ptr<Entity> entity_2 = std::make_shared<Decorator>(entity_1);

  entity_2->test();
}
