
#include <cassert>
#include <iostream>
#include <string>

class Person {
public:
  Person(const std::string& name, int age, int grade) : name_(name), age_(age), grade_(grade) {}

  std::string& get_name() {
    return name_;
  }

  int get_age() {
    return age_;
  }

  int get_grade() {
    return grade_;
  }
private:
  std::string name_;
  int age_;
  int grade_;
};

class Builder {
public:
  Builder() : age_(0), grade_(0) {}
  Builder& name(const std::string& name_builder) {
    name_ = name_builder;
    return *this;
  }

  Builder& age(int age) {
    age_ = age;
    return *this;
  }

  Builder& grade(int grade) {
    grade_ = grade;
    return *this;
  }

  Person get() {
    return Person(name_, age_, grade_);
  }
private:
  std::string name_;
  int age_;
  int grade_;
};

int main() {
  Builder builder;
  auto person = builder.name("Ivan").age(25).grade(10).get();
  assert(person.get_age() == 25);
  assert(person.get_grade() == 10);
}
