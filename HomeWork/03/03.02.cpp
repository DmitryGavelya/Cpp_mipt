// Реализуйте классы Triangle, Square и Circle для представления треугольников, квадратов и окружностей.
// Реализуйте в классе Triangle три поля типа double для хранения длин трех сторон, в классе Square одно
// поле типа double для хранения длины одной стороны, в классе Circle одно поле типа double для хранения
// радиуса. Реализуйте алгоритмы вычисления периметра и площади указанных фигур как публичные функции-
// члены соответствующих классов. Используйте стандартную константу std::numbers::pi для класса Circle


#include <numbers>
#include <cmath>
#include <iostream>

class Triangle {

  public:
    Triangle(double first, double second, double third) : first_side(first), second_side(second), third_side(third) {
      if (first + second <= third || first + third <= second || second + third <= first) {
        std::cout << "it is not a triangle";
      }
    }

    double first_side = 0.0;
    double second_side = 0.0;
    double third_side = 0.0;

    double perimeter() const {
      return first_side + second_side + third_side;
    }

    double area() const {
      double p = (first_side + second_side + third_side) / 2;

      return std::sqrt(std::max(0.0, p * (p - first_side) * (p - second_side) * (p - third_side)));
    }
};

class Square {

  public:
    Square(double side) : side(side) {}

    double side = 0.0;

    double perimeter() const {
      return side * 4;
    }

    double area() const {
      return side * side;
    }
};

class Circle {

  public:
    Circle(double radius) : radius(radius) {}

    double radius = 0;

    double perimeter() const {
      return 2 * std::numbers::pi * radius;
    }

    double area() const {
      return std::numbers::pi * radius * radius;
    }
};

int main() {

}
