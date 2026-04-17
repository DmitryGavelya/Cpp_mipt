#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "08.19.hpp"

auto make_strings(std::size_t size_1, std::size_t size_2) {
  std::set<std::string> strings;
  std::string string(size_2, '_');
  std::uniform_int_distribution distribution(97, 122);
  std::default_random_engine engine;

  while (std::size(strings) < size_1) {
    for (auto &element : string) {
      element = distribution(engine);
    }
    strings.insert(string);
  }
  return strings;
}

std::size_t RSHash(const std::string &str) {
  std::size_t b = 378551;
  std::size_t a = 63689;
  std::size_t hash = 0;
  for (char c : str) {
    hash = hash * a + c;
    a = a * b;
  }
  return hash;
}

std::size_t JSHash(const std::string &str) {
  std::size_t hash = 1315423911;
  for (char c : str) {
    hash ^= ((hash << 5) + c + (hash >> 2));
  }
  return hash;
}

std::size_t PJWHash(const std::string &str) {
  std::size_t BitsInUnsignedInt = (std::size_t)(sizeof(std::size_t) * 8);
  std::size_t ThreeQuarters = (std::size_t)((BitsInUnsignedInt * 3) / 4);
  std::size_t OneEighth = (std::size_t)(BitsInUnsignedInt / 8);
  std::size_t HighBits = (std::size_t)(0xFFFFFFFF)
                         << (BitsInUnsignedInt - OneEighth);
  std::size_t hash = 0;
  std::size_t test = 0;

  for (char c : str) {
    hash = (hash << OneEighth) + c;
    if ((test = hash & HighBits) != 0) {
      hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }
  return hash;
}

std::size_t ELFHash(const std::string &str) {
  std::size_t hash = 0;
  std::size_t x = 0;
  for (char c : str) {
    hash = (hash << 4) + c;
    if ((x = hash & 0xF0000000L) != 0) {
      hash ^= (x >> 24);
    }
    hash &= ~x;
  }
  return hash;
}

std::size_t BKDRHash(const std::string &str) {
  std::size_t seed = 131;
  std::size_t hash = 0;
  for (char c : str) {
    hash = (hash * seed) + c;
  }
  return hash;
}

std::size_t SDBMHash(const std::string &str) {
  std::size_t hash = 0;
  for (char c : str) {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }
  return hash;
}

std::size_t DJBHash(const std::string &str) {
  std::size_t hash = 5381;
  for (char c : str) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

std::size_t DEKHash(const std::string &str) {
  std::size_t hash = str.length();
  for (char c : str) {
    hash = ((hash << 5) ^ (hash >> 27)) ^ c;
  }
  return hash;
}

std::size_t APHash(const std::string &str) {
  std::size_t hash = 0xAAAAAAAA;
  for (std::size_t i = 0; i < str.length(); i++) {
    if ((i & 1) == 0) {
      hash ^= ((hash << 7) ^ str[i] ^ (hash >> 3));
    } else {
      hash ^= (~((hash << 11) ^ str[i] ^ (hash >> 5)));
    }
  }
  return hash;
}

int main() {
  std::vector<
      std::pair<std::string, std::function<std::size_t(const std::string &)>>>
      hash_funcs = {{"RS", RSHash},   {"JS", JSHash},     {"PJW", PJWHash},
                    {"ELF", ELFHash}, {"BKDR", BKDRHash}, {"SDBM", SDBMHash},
                    {"DJB", DJBHash}, {"DEK", DEKHash},   {"AP", APHash}};

  auto strings = make_strings(1'000'000, 10);

  Python python;

  try {
    auto const &local = python.local();
    boost::python::exec("from script import make_plot", local, local);

    for (const auto &[name, func] : hash_funcs) {
      std::set<std::size_t> hashes;
      std::string points;

      for (auto i = 0uz; auto const &string : strings) {
        hashes.insert(func(string));

        if (i++ % 1'000 == 0) {
          points += i == 1 ? "" : ",";
          points += std::to_string(i - 1) + ',';
          points += std::to_string(i - std::size(hashes));
        }
      }
      local["make_plot"](points.c_str(), name.c_str());
    }
  } catch (boost::python::error_already_set const &) {
    std::cerr << "main : " << Python::exception() << '\n';
  }
}
