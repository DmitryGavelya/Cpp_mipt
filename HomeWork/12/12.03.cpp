#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::string_view longest_palindromic_substring(std::string_view s) {
  if (s.empty()) return "";

  size_t n = s.length();
  std::vector<bool> dp(n * n, false);

  size_t start = 0;
  size_t max_len = 1;

  for (size_t i = 0; i < n; ++i) {
    dp[i * n + i] = true;
  }

  for (size_t i = 0; i < n - 1; ++i) {
    if (s[i] == s[i + 1]) {
      dp[i * n + (i + 1)] = true;
      start = i;
      max_len = 2;
    }
  }

  for (size_t len = 3; len <= n; ++len) {
    for (size_t i = 0; i < n - len + 1; ++i) {
      size_t j = i + len - 1;

      if (s[i] == s[j] && dp[(i + 1) * n + (j - 1)]) {
        dp[i * n + j] = true;

        if (len > max_len) {
          start = i;
          max_len = len;
        }
      }
    }
  }

  return s.substr(start, max_len);
}

int main() {
  std::string test1 = "babad";
  std::cout << longest_palindromic_substring(test1) << "\n";

  std::string test2 = "cbbd";
  std::cout << longest_palindromic_substring(test2) << "\n";

  std::string test3 = "a";
  std::cout << longest_palindromic_substring(test3) << "\n";

  std::string test4 = "ac";
  std::cout << longest_palindromic_substring(test4) << "\n";
}
