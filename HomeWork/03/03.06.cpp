// Доработайте Ваше предыдущее решение задачи 03.02 таким образом, чтобы все реализованные классы явля-
//   лись наследниками одного базового класса. Реализуйте абстрактный базовый класс Shape для представления
//   интерфейса геометрических фигур. Реализуйте в классе Shape виртуальный деструктор и две публичные чи-
//   сто виртуальные функции-члены perimeter и area для вычисления периметра и площади соответственно.
//   Реализуйте производные классы Triangle, Square и Circle, которые являются наследниками интерфейса
//   класса Shape. Используйте спецификатор override при переопределении наследуемых виртуальных функций-
//   членов perimeter и area вклассахTriangle,Square иCircle.Используйте спецификатор final длязапрета
//   наследования от классов Square и Circle и для запрета переопределения наследуемых виртуальных функций-
//   членов perimeter и area в возможных наследниках класса Triangle. Используйте стандартный контейнер
 // std::vector для хранения экземпляров классов Triangle, Square и
//   Circle через указатели на класс Shape.

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <numbers>

class Shape {
  public:
    virtual ~Shape() = default;
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
};

class Triangle : public Shape {
  public:
    Triangle(double first, double second, double third) : first_side(first), second_side(second), third_side(third) {
      if (first + second <= third || first + third <= second || second + third <= first) {
        std::cout << "it is not a triangle";
      }
    }

    double perimeter() const final override {
      return first_side + second_side + third_side;
    }

    double area() const final override {
      double p = (first_side + second_side + third_side) / 2;

      return std::sqrt(std::max(0.0, p * (p - first_side) * (p - second_side) * (p - third_side)));
    }

  private:
    double first_side = 0.0;
    double second_side = 0.0;
    double third_side = 0.0;
};

class Square final : public Shape {
  public:
    Square(double side) : side(side) {}

    double perimeter() const override {
      return side * 4;
    }

    double area() const override {
      return side * side;
    }

  private:
    double side = 0.0;
};
class Circle final : public Shape {
  public:
    Circle(double radius) : radius(radius) {}

    double perimeter() const override {
      return 2 * std::numbers::pi * radius;
    }

    double area() const override {
      return std::numbers::pi * radius * radius;
    }

  private:
    double radius = 0;
};

int main() {
  std::vector<Shape *> shapes;
}