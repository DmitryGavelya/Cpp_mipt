#include <limits>

template <int X> struct Fibonacci {
  static_assert(X >= 1, "N must be positive");

  static_assert(Fibonacci<X - 1>::value <= (std::numeric_limits<int>::max() - Fibonacci<X - 2>::value),
                "Integer overflow");
  static constexpr int value = Fibonacci<X - 1> ::value + Fibonacci<X - 2> :: value;
};


template<> struct Fibonacci<1> {
  static constexpr int value = 1;
};

template<> struct Fibonacci<2> {
  static constexpr int value = 1;
};

template < int X > auto const fibonacci_v = Fibonacci < X > ::value;

int main() {
  static_assert(fibonacci_v<1> == 1);
  static_assert(fibonacci_v<2> == 1);

  static_assert(fibonacci_v<3> == 2);
  static_assert(fibonacci_v<4> == 3);
  static_assert(fibonacci_v<5> == 5);
  static_assert(fibonacci_v<6> == 8);
  static_assert(fibonacci_v<7> == 13);

  static_assert(fibonacci_v<10> == 55);
}