#include <cassert>
#include <vector>
#include <memory>

class Entity {
public:
  virtual ~Entity() = default;
  virtual int test() const = 0;
};

class Client : public Entity {
public:
  int test() const override { return 1; }
};

class Server : public Entity {
public:
  int test() const override { return 2; }
};

class Composite : public Entity {
public:
  void add(std::shared_ptr<Entity> entity) {
    m_entities.push_back(entity);
  }

  int test() const override {
    auto x = 0;
    for (const auto& entity : m_entities) {
      if (entity) x += entity->test();
    }
    return x;
  }

private:
  std::vector<std::shared_ptr<Entity>> m_entities;
};

std::shared_ptr<Entity> make_composite(size_t size_1, size_t size_2) {
  auto composite = std::make_shared<Composite>();
  for (size_t i = 0; i < size_1; ++i) { composite->add(std::make_shared<Client>()); }
  for (size_t i = 0; i < size_2; ++i) { composite->add(std::make_shared<Server>()); }
  return composite;
}

int main() {
  auto composite = std::make_shared<Composite>();

  for (size_t i = 0; i < 5; ++i) {
    composite->add(make_composite(1, 1));
  }

  std::shared_ptr<Entity> entity = composite;
  assert(entity->test() == 15);
}
