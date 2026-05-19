//////////////////////////////////////////////////////////////////////////////

#include <functional>
#include <vector>
#include <iostream>
#include <string>

#include <boost/dll.hpp>

#include "library.hpp"

////////////////////////////////////////////////////////////////////////////////

int main()
{
  std::cout << "=== Static Library ===\n";
  library::test();

  std::cout << "\n=== Dynamic Libraries ===\n";
  std::cout << "Available versions: 1 or 2\n";

  std::string version;
  std::cout << "\nEnter library version (1 or 2): ";
  std::getline(std::cin, version);

  std::string path;
  if (version == "1") {
    path = "libshared_v1.so";
  } else if (version == "2") {
    path = "libshared_v2.so";
  } else {
    std::cerr << "Invalid version. Using version 1.\n";
    path = "libshared_v1.so";
  }

  std::cout << "\nLoading library: " << path << "\n\n";

  try {
    std::vector<std::function<void()>> functions;

    functions.push_back(boost::dll::import_symbol<void()>(path, "test_v1"));
    functions.push_back(boost::dll::import_symbol<void()>(path, "test_v2"));

    std::cout << "Calling imported functions:\n";
    for (auto const & function : functions) {
      function();
    }

    std::cout << "\nCalling test_v3:\n";
    auto test_v3_func = boost::dll::import_symbol<void()>(path, "test_v3");
    test_v3_func();
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
