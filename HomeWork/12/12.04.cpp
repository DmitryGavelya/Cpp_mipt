#include <algorithm>
#include <cassert>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

using namespace std::literals;

int main()
{
  auto text = R"(Hello, reach me at john.doe@gmail.com or support@company.org, also try admin@mail.ru)"s;

  auto begin = std::cbegin(text);

  std::regex pattern(R"([a-zA-Z0-9._%+-]+@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");

  std::vector<std::string> results;

  auto lambda = [&results](auto const& match){ results.push_back(match); };

  std::ranges::for_each
  (
      std::sregex_token_iterator(begin, std::cend(text), pattern, { 0, 1 }),

      std::sregex_token_iterator(),

      lambda
  );

  assert(results == std::vector<std::string>
  ({
      "john.doe@gmail.com", "gmail.com",
      "support@company.org", "company.org",
      "admin@mail.ru", "mail.ru"
  }));
}
