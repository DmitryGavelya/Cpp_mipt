#include <print>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>

auto to_hex(std::vector<std::uint8_t> const & data) -> std::string
{
  std::stringstream ss;

  for (auto byte : data)
  {
    ss << std::hex << std::setw(2) << std::right << std::setfill('0') << static_cast<int>(byte);
  }

  return ss.str();
}

auto from_hex(std::string const & string) -> std::vector<std::uint8_t>
{
  std::vector<std::uint8_t> result;

  for (std::size_t i = 0; i < string.size(); i += 2)
  {
    auto hi = string[i];
    auto lo = string[i + 1];

    auto to_val = [](char c) -> std::uint8_t
    {
      if (c >= '0' && c <= '9') return c - '0';
      return c - 'a' + 10;
    };

    result.push_back((to_val(hi) << 4) | to_val(lo));
  }

  return result;
}

int main()
{
  std::vector<std::uint8_t> const data = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x23, 0xFF, 0x00 };

  auto hex = to_hex(data);

  std::print("to_hex   : {}\n", hex);

  auto bytes = from_hex(hex);

  std::print("from_hex : ");

  for (auto b : bytes)
  {
    std::print("{:02x} ", b);
  }

  std::print("\n");
}#include <print>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>

auto to_hex(std::vector<std::uint8_t> const & data) -> std::string
{
  std::stringstream ss;

  for (auto byte : data)
  {
    ss << std::hex << std::setw(2) << std::right << std::setfill('0') << static_cast<int>(byte);
  }

  return ss.str();
}

auto from_hex(std::string const & string) -> std::vector<std::uint8_t>
{
  std::vector<std::uint8_t> result;

  for (std::size_t i = 0; i < string.size(); i += 2)
  {
    auto hi = string[i];
    auto lo = string[i + 1];

    auto to_val = [](char c) -> std::uint8_t
    {
      if (c >= '0' && c <= '9') return c - '0';
      return c - 'a' + 10;
    };

    result.push_back((to_val(hi) << 4) | to_val(lo));
  }

  return result;
}

int main()
{
  std::vector<std::uint8_t> const data = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x23, 0xFF, 0x00 };

  auto hex = to_hex(data);

  std::print("to_hex   : {}\n", hex);

  auto bytes = from_hex(hex);

  std::print("from_hex : ");

  for (auto b : bytes)
  {
    std::print("{:02x} ", b);
  }

  std::print("\n");
}