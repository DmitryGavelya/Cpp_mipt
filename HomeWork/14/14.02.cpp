#include <algorithm>
#include <cassert>
#include <functional>
#include <future>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

///////////////////////////////////////////////////////////////////////////

template < std::ranges::view V, typename F >
auto for_each(V view, F function) -> void
{
  auto begin = std::begin(view), end = std::end(view);

  if (auto size = std::distance(begin, end), half = size / 2; size > 16)
  {
    std::ranges::subrange range(begin, std::next(begin, half));

    auto future = std::async(for_each < decltype(range), F >, range, function);

    for_each(std::ranges::subrange(std::end(range), end), function);

    future.get();
  }
  else
  {
    std::ranges::for_each(view, function);
  }
}

///////////////////////////////////////////////////////////////////////////

int main()
{
  std::vector < int > vector(1 << 10, 0);

  std::ranges::iota(vector, 1);

  std::atomic < long long > sum = 0;

  for_each(std::views::all(vector), [&sum](int x) { sum += x; });

  assert(sum == 524'800);

  std::print("main : sum = {}\n", sum.load());

  std::print("main : parallel for_each return type : {}\n", typeid(decltype(
      for_each(std::views::all(vector), [](int){})
  )).name());

  std::print("main : std::for_each    return type : {}\n", typeid(decltype(
      std::for_each(std::begin(vector), std::end(vector), [](int){})
  )).name());
}