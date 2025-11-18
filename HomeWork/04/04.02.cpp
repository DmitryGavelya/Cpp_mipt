// Реализуйтеалгоритмывычислениямаксимальногоиминимальногозначений,суммыисреднегоарифметичес-
//   кого всех аргументов типа double из пакета. Используйте вариативные шаблоны функций. Используйте выра-
//   жения свертки. Предполагайте, что пакет может содержать аргументы иных типов, которые следует игнориро-
//   вать. Предполагайте, что пакет содержит хотя бы один аргумент типа double. Реализуйте отдельные шаблоны
 // функций с полными специализациями для типа
//   double для обработки всех аргументов типа double из пакета.

#include <cassert>
#include <iostream>

class Utils {
  private:
    static int count_value(auto head) {
      if constexpr (std::is_same_v<decltype(head), double>) {
        return 1;
      }
      return 0;
    }

    static int count_value(auto head, auto... tail) {
      int count_item = count_value(tail...);
      if constexpr (std::is_same_v<decltype(head), double>) {
        count_item++;
      }

      return count_item;
    }
  public:
    static std::optional<double> max_value(auto head) {
      if constexpr (std::is_same_v<decltype(head), double>) {
        return head;
      }
      return std::nullopt;
    }

    static std::optional<double> max_value(auto head, auto ... tail) {
      std::optional<double> max_item = max_value(tail...);

      if (max_item.has_value()) {
        if constexpr (std::is_same_v<decltype(head), double>) {
          return std::max(head, max_item.value());
        }
        return max_item;
      }
      if constexpr (std::is_same_v<decltype(head), double>) {
        return head;
      }
      return std::nullopt;
    }

    static std::optional<double> min_value(auto head) {
      if constexpr (std::is_same_v<decltype(head), double>) {
        return head;
      }
      return std::nullopt;
    }

    static std::optional<double> min_value(auto head, auto ... tail) {
      std::optional<double> min_item = min_value(tail...);

      if (min_item.has_value()) {
        if constexpr (std::is_same_v<decltype(head), double>) {
          return std::min(head, min_item.value());
        }
        return min_item;
      }
      if constexpr (std::is_same_v<decltype(head), double>) {
        return head;
      }
      return std::nullopt;
    }

    static std::optional<double> sum_value(auto head) {
      if constexpr (std::is_same_v<decltype(head), double>) {
        return head;
      }
      return std::nullopt;
    }

    static std::optional<double> sum_value(auto head, auto ... tail) {
      std::optional<double> sum_item = sum_value(tail...);

      if (sum_item.has_value()) {
        if constexpr (std::is_same_v<decltype(head), double>) {
          return head + sum_item.value();
        }
        return sum_item;
      }
      if constexpr (std::is_same_v<decltype(head), double>) {
        return head;
      }
      return std::nullopt;
    }

  static std::optional<double> avg_value(auto head, auto ... tail) {
      int count_item = count_value(head, tail...);
      if (count_item > 0) {
        std::optional<double> sm = sum_value(head, tail...);
        return (sm.value() * 1.0) / count_item;
      }
      return std::nullopt;
    }
};

int main() {
  std::optional<double> answer = Utils::avg_value(1.0, 3.0, 3, 4);
  if (answer.has_value()) {
    assert(answer.value() == 2);
  }
}