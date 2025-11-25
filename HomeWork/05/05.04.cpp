#include <iostream>
#include <string>
#include <vector>

class UnitState {
public:
  virtual ~UnitState() = default;
  virtual void execute() = 0;
  virtual std::string getName() = 0;
};

class IdleState : public UnitState {
public:
  void execute() override { std::cout << "Unit is idle\n"; }
  std::string getName() override { return "Idle"; }
};

class AttackState : public UnitState {
public:
  void execute() override { std::cout << "Unit is attacking!\n"; }
  std::string getName() override { return "Attack"; }
};

class MoveState : public UnitState {
public:
  void execute() override { std::cout << "Unit is moving\n"; }
  std::string getName() override { return "Move"; }
};

class Unit {
public:
  virtual ~Unit() = default;
  virtual void display() = 0;
  virtual void setState(UnitState *state) { currentState = state; }
  virtual void executeState() {
    if (currentState) {
      currentState->execute();
    }
  }

protected:
  UnitState *currentState = nullptr;
};

class Warrior : public Unit {
public:
  void display() override { std::cout << "Warrior unit\n"; }
};

class Archer : public Unit {
public:
  void display() override { std::cout << "Archer unit\n"; }
};

class Mage : public Unit {
public:
  void display() override { std::cout << "Mage unit\n"; }
};

class UnitFactory {
public:
  static Unit *createUnit(const std::string &type) {
    if (type == "warrior") {
      return new Warrior();
    }
    if (type == "archer") {
      return new Archer();
    }
    if (type == "mage") {
      return new Mage();
    }
    return nullptr;
  }
};

class ArmyComposite : public Unit {
private:
  std::vector<Unit *> units;
  std::string name;

public:
  ArmyComposite(const std::string &armyName) : name(armyName) {}

  ~ArmyComposite() {
    for (Unit *unit : units) {
      delete unit;
    }
    units.clear();
  }

  void addUnit(Unit *unit) { units.push_back(unit); }

  void display() override {
    for (Unit *unit : units) {
      unit->display();
    }
  }

  void executeState() override {
    if (currentState) {
      currentState->execute();
    }
    for (Unit *unit : units) {
      unit->executeState();
    }
  }
};

int main() {
  Unit *warrior = UnitFactory::createUnit("warrior");
  Unit *archer = UnitFactory::createUnit("archer");
  Unit *mage = UnitFactory::createUnit("mage");

  warrior->setState(new AttackState());
  archer->setState(new MoveState());
  mage->setState(new IdleState());

  ArmyComposite *mainArmy = new ArmyComposite("Main Army");
  mainArmy->addUnit(warrior);
  mainArmy->addUnit(archer);

  ArmyComposite *reserveArmy = new ArmyComposite("Reserve Army");
  reserveArmy->addUnit(mage);
  reserveArmy->setState(new IdleState());

  mainArmy->display();
  reserveArmy->display();

  mainArmy->executeState();
  reserveArmy->executeState();

  delete mainArmy;
  delete reserveArmy;
}