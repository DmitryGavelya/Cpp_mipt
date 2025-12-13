#include <numbers>
#include <cmath>
#include <iostream>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
};

class Circle : public Shape {
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

class Polygon : public Shape {
public:
    virtual ~Polygon() = default;
};

class Triangle : public Polygon {
public:
    Triangle(double first, double second, double third) 
        : first_side(first), second_side(second), third_side(third) {
        if (first + second <= third || first + third <= second || second + third <= first) {
            std::cout << "It is not a triangle\n";
        }
    }
    
    double perimeter() const override {
        return first_side + second_side + third_side;
    }
    
    double area() const override {
        double p = (first_side + second_side + third_side) / 2;
        return std::sqrt(std::max(0.0, p * (p - first_side) * (p - second_side) * (p - third_side)));
    }

private:
    double first_side = 0.0;
    double second_side = 0.0;
    double third_side = 0.0;
};

class Rectangle : public Polygon {
public:
    Rectangle(double width, double height) : width(width), height(height) {}
    
    double perimeter() const override {
        return 2 * (width + height);
    }
    
    double area() const override {
        return width * height;
    }

protected:
    double width = 0.0;
    double height = 0.0;
};

class Square : public Rectangle {
public:
    Square(double side) : Rectangle(side, side) {}
};

int main() {
    std::vector<Shape*> shapes;
    
    shapes.push_back(new Circle(5.0));
    shapes.push_back(new Triangle(3.0, 4.0, 5.0));
    shapes.push_back(new Triangle(5.0, 5.0, 5.0));
    shapes.push_back(new Rectangle(4.0, 6.0));
    shapes.push_back(new Square(4.0));
    
    for (size_t i = 0; i < shapes.size(); ++i) {
        std::cout << "Shape " << (i + 1) << ":\n";
        std::cout << "Perimeter: " << shapes[i]->perimeter() << "\n";
        std::cout << "Area: " << shapes[i]->area() << "\n";
    }
    
    for (size_t i = 0; i < shapes.size(); ++i) {
        delete shapes[i];
    }
    shapes.clear();
    
    return 0;
}
