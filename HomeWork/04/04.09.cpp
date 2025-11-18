#include <type_traits>

template <typename T> struct add_const {
  using type = const T;
};

template <typename T> struct add_const<T&> {
  using type = T&;
};

template <typename T> struct add_const<T&&> {
  using type = T&&;
};

template<typename T> using add_const_t = typename add_const<T>::type;

template <typename T> struct remove_const {
  using type = T;
};

template <typename T> struct remove_const<const T> {
  using type = T;
};

template<typename T> using remove_const_t = typename remove_const<T>::type;

template <bool B, typename T, typename F> struct conditional;

template <typename T, typename F> struct conditional<true, T, F> {
  using type = T;
};

template <typename T, typename F> struct conditional<false, T, F> {
  using type = F;
};

template<bool B, typename T, typename F> using conditional_t = typename conditional<B, T, F>::type;

template <typename T> struct decay;

template <typename T> struct decay {
  using type = T;
};

template <typename T> struct decay<T&> {
  using type = T;
};

template <typename T> struct decay<T&&> {
  using type = T;
};

template <typename T, std::size_t N>
struct decay<T[N]> {
  using type = T*;
};

template <typename T>
struct decay<T[]> {
  using type = T*;
};

template<typename R, typename... Args>
struct decay<R(Args...)> {
  using type = R(*)(Args...);
};

template<typename T> using decay_t = typename decay<T>::type;

template<typename T>
struct is_class {
  private:
    template<typename U> static std::true_type test(int U::*);
    template<typename U> static std::false_type test(...);
  public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template<typename T> constexpr bool is_class_v = is_class<T>::value;


class MyClass {};

int main() {
  // add_const
  static_assert(std::is_same_v<add_const_t<int>, const int>);
  static_assert(std::is_same_v<add_const_t<int&>, int&>);

  // remove_const
  static_assert(std::is_same_v<remove_const_t<const int>, int>);
  static_assert(std::is_same_v<remove_const_t<int>, int>);

  // conditional
  static_assert(std::is_same_v<conditional_t<true, int, double>, int>);
  static_assert(std::is_same_v<conditional_t<false, int, double>, double>);

  // decay
  static_assert(std::is_same_v<decay_t<int&>, int>);
  static_assert(std::is_same_v<decay_t<int[5]>, int*>);

  // is_class
  static_assert(is_class_v<MyClass> == true);
  static_assert(is_class_v<int> == false);

}