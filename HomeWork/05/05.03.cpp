#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class GameStrategy {
public:
    virtual ~GameStrategy() = default;
    virtual void execute() const = 0;
    virtual std::string getName() const = 0;
};

class AggressiveStrategy : public GameStrategy {
public:
    void execute() const override {
        std::cout << "  [Strategy] Атакуем врагов!\n";
    }
    std::string getName() const override { return "Aggressive"; }
};

class DefensiveStrategy : public GameStrategy {
public:
    void execute() const override {
        std::cout << "  [Strategy] Защищаемся!\n";
    }
    std::string getName() const override { return "Defensive"; }
};

class BalancedStrategy : public GameStrategy {
public:
    void execute() const override {
        std::cout << "  [Strategy] Уравновешенная тактика\n";
    }
    std::string getName() const override { return "Balanced"; }
};

class GameObject {
public:
    virtual ~GameObject() = default;

    void update() {
        try {
            prepare();
            execute();
            cleanup();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    virtual std::string getType() const = 0;
    virtual void describe() const = 0;

protected:
    virtual void prepare() {
        std::cout << "Подготовка " << getType() << "\n";
    }

    virtual void execute() {
        std::cout << "Выполнение " << getType() << "\n";
    }

    virtual void cleanup() {
        std::cout << "Завершение " << getType() << "\n";
    }
};

class Unit : public GameObject {
private:
  const std::string m_name;
  GameStrategy *m_strategy;

public:
  Unit(const std::string &name, GameStrategy *strat)
      : m_name(name), m_strategy(strat) {}

  ~Unit() override = default;

  std::string getType() const override { return "Unit(" + m_name + ")"; }

  void describe() const override {
    std::cout << "Unit: " << m_name << " with strategy "
              << m_strategy->getName() << "\n";
  }

protected:
  void execute() override {
    std::cout << "  [TM] Unit " << m_name << " выполняет действие\n";
    m_strategy->execute();
  }
};

class CompositeUnit : public GameObject {
private:
    const std::string m_name;
    std::vector<GameObject*> m_children;

public:
    explicit CompositeUnit(const std::string& name) : m_name(name) {}

    ~CompositeUnit() override = default;

    void add(GameObject* child) {
        m_children.push_back(child);
    }

    void remove(GameObject* child) {
        for (size_t i = 0; i < m_children.size(); ++i) {
            if (m_children[i] == child) {
                m_children.erase(m_children.begin() + i);
                return;
            }
        }
    }

    std::string getType() const override { return "CompositeUnit(" + m_name + ")"; }

    void describe() const override {
        std::cout << "CompositeUnit: " << m_name << " with "
                  << m_children.size() << " children\n";
    }

    size_t getChildCount() const { return m_children.size(); }

protected:
    void execute() override {
        std::cout << "  [Composite] " << m_name << " координирует подразделения:\n";
        for (GameObject* child : m_children) {
            child->update();
        }
    }
};

class GameObjectBuilder {
private:
    std::string m_name;
    GameStrategy* m_strategy;
    std::vector<GameObject*> m_components;
    bool m_name_set = false;

public:
    GameObjectBuilder() : m_strategy(nullptr) {}

    GameObjectBuilder& setName(const std::string& name) {
        m_name = name;
        m_name_set = true;
        return *this;
    }

    GameObjectBuilder& setStrategy(GameStrategy* strat) {
        m_strategy = strat;
        return *this;
    }

    GameObjectBuilder& addComponent(GameObject* comp) {
        m_components.push_back(comp);
        return *this;
    }

    GameObject* build() {

        if (m_components.empty()) {
            if (!m_strategy) {
                static BalancedStrategy defaultStrategy;
                m_strategy = &defaultStrategy;
            }
            return new Unit(m_name, m_strategy);
        }
        CompositeUnit* composite = new CompositeUnit(m_name);
        for (GameObject* comp : m_components) {
            composite->add(comp);
        }
        return composite;
    }
};


int main() {
    AggressiveStrategy aggStrat;
    DefensiveStrategy defStrat;
    BalancedStrategy balStrat;

    GameObject* warrior = GameObjectBuilder()
        .setName("Warrior")
        .setStrategy(&aggStrat)
        .build();

    GameObject* archer = GameObjectBuilder()
        .setName("Archer")
        .setStrategy(&defStrat)
        .build();

    GameObject* mage = GameObjectBuilder()
        .setName("Mage")
        .setStrategy(&balStrat)
        .build();

    GameObject* army = GameObjectBuilder()
        .setName("Army")
        .addComponent(warrior)
        .addComponent(archer)
        .addComponent(mage)
        .build();

    army->update();

    warrior->update();

    delete warrior;
    delete archer;
    delete mage;
    delete army;


    return 0;
}