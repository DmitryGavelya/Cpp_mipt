#include <memory>

class Entity {
public:
  virtual ~Entity() = default;
};

class Client : public Entity {};
class Server : public Entity {};

class Factory {
public:
  virtual ~Factory() = default;
  virtual std::unique_ptr<Entity> make_entity() const = 0;
};

class Factory_Client : public Factory {
public:
  std::unique_ptr<Entity> make_entity() const override {
    return std::make_unique<Client>();
  }
};

int main() {
  std::unique_ptr<Factory> factory = std::make_unique<Factory_Client>();
  factory->make_entity();
}
