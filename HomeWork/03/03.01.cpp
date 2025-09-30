// Реализуйте структуру Rectangle для представления прямоугольников со сторонами, которые параллельны
// осям координатной плоскости. Реализуйте в структуре Rectangle поля типа double для хранения коорди-
// нат левого верхнего и правого нижнего углов прямоугольника. Используйте систему координат, в которой ось
// абсцисс направлена вправо, а ось ординат направлена вниз. Реализуйте алгоритм вычисления площади пересе-
// чения нескольких прямоугольников как свободную функцию. Рассмотрите случаи вырожденного и пустого пе-
// ресечения. Реализуйте алгоритм вычисления минимального ограничивающего прямоугольника как свободную
// функцию. Используйте стандартный контейнер std::vector для хранения экземпляров структуры Rectangle.

#include <iostream>
#include <algorithm>
#include <vector>

struct Pairs {

  Pairs(double first = 0, double second = 0) : first_number(first), second_number(second) {}

  double first_number;
  double second_number;
};

struct Rectangle
{
  Rectangle(const Pairs & left_upper_corner, const Pairs & right_down_corner) : left_upper_corner(left_upper_corner), right_down_corner(right_down_corner) {}

  [[nodiscard]] double get_left() const { return left_upper_corner.first_number; }
  [[nodiscard]] double get_top() const { return left_upper_corner.second_number; }
  [[nodiscard]] double get_right() const { return right_down_corner.first_number; }
  [[nodiscard]] double get_bottom() const { return right_down_corner.second_number; }

  [[nodiscard]] double get_width() const { return get_right() - get_left(); }
  [[nodiscard]] double get_height() const { return get_bottom() - get_top(); }
  [[nodiscard]] double get_area() const { return get_width() * get_height(); }

  private:
    Pairs left_upper_corner;
    Pairs right_down_corner;

};

bool intersects(const Rectangle& first_rectangle, const Rectangle& second_rectangle) {
  double first_left = first_rectangle.get_left();
  double first_top = first_rectangle.get_top();
  double first_right = first_rectangle.get_right();
  double first_bottom = first_rectangle.get_bottom();

  double second_left = second_rectangle.get_left();
  double second_top = second_rectangle.get_top();
  double second_right = second_rectangle.get_right();
  double second_bottom = second_rectangle.get_bottom();

  bool no_horizontal_overlap = (first_right <= second_left) || (second_right <= first_left);
  bool no_vertical_overlap = (first_bottom <= second_top) || (second_bottom <= first_top);

  if (no_horizontal_overlap || no_vertical_overlap) {
    return false;
  }

  return true;
}

double intersection_area(const Rectangle& first, const Rectangle& second) {
  if (!intersects(first, second)) {
    return 0.0;
  }

  double first_left = first.get_left();
  double first_top = first.get_top();
  double first_right = first.get_right();
  double first_bottom = first.get_bottom();

  double second_left = second.get_left();
  double second_top = second.get_top();
  double second_right = second.get_right();
  double second_bottom = second.get_bottom();

  double overlap_left = std::max(first_left, second_left);
  double overlap_top = std::max(first_top, second_top);
  double overlap_right = std::min(first_right, second_right);
  double overlap_bottom = std::min(first_bottom, second_bottom);

  double overlap_width = overlap_right - overlap_left;
  double overlap_height = overlap_bottom - overlap_top;
  double area = overlap_width * overlap_height;

  return area;
}

double intersection_area(const std::vector<Rectangle>& rectangles) {
  if (rectangles.empty()) return 0.0;

  Rectangle current_intersection = rectangles[0];

  for (std::size_t i = 1; i < rectangles.size(); ++i) {
    if (!intersects(current_intersection, rectangles[i])) {
      return 0.0;
    }

    double left = std::max(current_intersection.get_left(), rectangles[i].get_left());
    double top = std::max(current_intersection.get_top(), rectangles[i].get_top());
    double right = std::min(current_intersection.get_right(), rectangles[i].get_right());
    double bottom = std::min(current_intersection.get_bottom(), rectangles[i].get_bottom());

    current_intersection = Rectangle(Pairs(left, top), Pairs(right, bottom));
  }

  return current_intersection.get_area();
}

Rectangle bounding_box(const Rectangle& first, const Rectangle& second) {
  double min_left = std::min(first.get_left(), second.get_left());
  double min_top = std::min(first.get_top(), second.get_top());
  double max_right = std::max(first.get_right(), second.get_right());
  double max_bottom = std::max(first.get_bottom(), second.get_bottom());

  return Rectangle(Pairs(min_left, min_top),
                   Pairs(max_right, max_bottom));
}

Rectangle bounding_box(const std::vector<Rectangle>& rectangles) {
  if (rectangles.empty()) {
    return Rectangle(Pairs(0,0), Pairs(0,0));
  }

  double min_left = rectangles[0].get_left();
  double min_top = rectangles[0].get_top();
  double max_right = rectangles[0].get_right();
  double max_bottom = rectangles[0].get_bottom();

  for (size_t i = 1; i < rectangles.size(); ++i) {
    min_left = std::min(min_left, rectangles[i].get_left());
    min_top = std::min(min_top, rectangles[i].get_top());
    max_right = std::max(max_right, rectangles[i].get_right());
    max_bottom = std::max(max_bottom, rectangles[i].get_bottom());
  }

  return Rectangle(Pairs(min_left, min_top), Pairs(max_right, max_bottom));
}

int main() {

  std::vector<Rectangle> rectangles;

  std::size_t count_rectangles = 0;

  std::cin >> count_rectangles;

  for (std::size_t i = 0; i < count_rectangles; ++i) {
    double x1 = 0.0;
    double y1 = 0.0;
    double x2 = 0.0;
    double y2 = 0.0;

    std::cin >> x1 >> y1 >> x2 >> y2;
    rectangles.emplace_back(Pairs(x1,y1), Pairs(x2, y2));
  }

  std::cout << intersection_area(rectangles);
}
