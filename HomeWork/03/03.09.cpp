#include <array>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <cassert>

class IPv4 {
  private:
    std::array<std::uint8_t, 4> data;
  public:
    IPv4() : data{0, 0, 0, 0} {}
    IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d) : data{a, b, c, d} {}
    
    IPv4(const IPv4& other) = default;
    IPv4& operator=(const IPv4& other) = default;

    IPv4& operator++() {
      for (int i = 3; i >= 0; --i) {
        if (data[i] < 255) {
          ++data[i];
          break;
        }
        data[i] = 0;
      }
      return *this;
    }

    IPv4 operator++(int) {
        IPv4 temp(*this);
        ++(*this);
        return temp;
    }

    IPv4& operator--() {
        for (int i = 3; i >= 0; --i) {
          if (data[i] > 0) {
            --data[i];
            break;
          }
          data[i] = 255;
        }
        return *this;
    }

    IPv4 operator--(int) {
        IPv4 temp(*this);
        --(*this);
        return temp;
    }

    friend bool operator==(const IPv4& lhs, const IPv4& rhs) {
        return lhs.data == rhs.data;
    }

    friend bool operator!=(const IPv4& lhs, const IPv4& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const IPv4& lhs, const IPv4& rhs) {
        for (size_t i = 0; i < 4; ++i) {
          if (lhs.data[i] < rhs.data[i]) return true;
          if (lhs.data[i] > rhs.data[i]) return false;
        }
        return false;
    }

    friend bool operator>(const IPv4& lhs, const IPv4& rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const IPv4& lhs, const IPv4& rhs) {
        return !(lhs > rhs);
    }

    friend bool operator>=(const IPv4& lhs, const IPv4& rhs) {
        return !(lhs < rhs);
    }

    friend std::istream& operator>>(std::istream& is, IPv4& ip) {
        char dot1, dot2, dot3;
        int a, b, c, d;

        if (is >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d) {
            if (dot1 != '.' || dot2 != '.' || dot3 != '.'
              || a < 0 || a > 255 || b < 0 || b > 255 ||
              c < 0 || c > 255 || d < 0 || d > 255) {
                is.setstate(std::ios::failbit);
            } else {
                ip.data[0] = static_cast<std::uint8_t>(a);
                ip.data[1] = static_cast<std::uint8_t>(b);
                ip.data[2] = static_cast<std::uint8_t>(c);
                ip.data[3] = static_cast<std::uint8_t>(d);
            }
        }
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
      os << static_cast<int>(ip.data[0]) << "."
         << static_cast<int>(ip.data[1]) << "."
         << static_cast<int>(ip.data[2]) << "."
         << static_cast<int>(ip.data[3]);
      return os;
    }
};

void test_constructors() {
    IPv4 ip1;
    IPv4 ip2(192, 168, 0, 1);
    IPv4 ip3 = ip2;

    assert(ip1 == IPv4(0, 0, 0, 0));
    assert(ip2 == IPv4(192, 168, 0, 1));
    assert(ip3 == ip2);
}

void test_prefix_increment() {
    IPv4 ip(192, 168, 0, 5);
    IPv4 result = ++ip;
    assert(ip == IPv4(192, 168, 0, 6));
    assert(result == ip);
}

void test_postfix_increment() {
    IPv4 ip(192, 168, 0, 5);
    IPv4 result = ip++;
    assert(ip == IPv4(192, 168, 0, 6));
    assert(result == IPv4(192, 168, 0, 5));
}

void test_byte_overflow() {
    IPv4 ip1(192, 168, 0, 255);
    ++ip1;
    assert(ip1 == IPv4(192, 168, 1, 0));

    IPv4 ip2(192, 168, 255, 255);
    ++ip2;
    assert(ip2 == IPv4(192, 169, 0, 0));

    IPv4 ip3(255, 255, 255, 255);
    ++ip3;
    assert(ip3 == IPv4(0, 0, 0, 0));
}

void test_prefix_decrement() {
    IPv4 ip(192, 168, 0, 5);
    IPv4 result = --ip;
    assert(ip == IPv4(192, 168, 0, 4));
    assert(result == ip);
}

void test_postfix_decrement() {
    IPv4 ip(192, 168, 0, 5);
    IPv4 result = ip--;
    assert(ip == IPv4(192, 168, 0, 4));
    assert(result == IPv4(192, 168, 0, 5));
}

void test_byte_underflow() {
    IPv4 ip1(192, 168, 0, 0);
    --ip1;
    assert(ip1 == IPv4(192, 167, 255, 255));

    IPv4 ip2(192, 0, 0, 0);
    --ip2;
    assert(ip2 == IPv4(191, 255, 255, 255));

    IPv4 ip3(0, 0, 0, 0);
    --ip3;
    assert(ip3 == IPv4(255, 255, 255, 255));
}

void test_equality() {
    IPv4 ip1(192, 168, 0, 1);
    IPv4 ip2(192, 168, 0, 1);
    IPv4 ip3(192, 168, 0, 2);

    assert(ip1 == ip2);
    assert(!(ip1 != ip2));
    assert(ip1 != ip3);
    assert(!(ip1 == ip3));
}

void test_comparison() {
    IPv4 ip1(192, 168, 0, 1);
    IPv4 ip2(192, 168, 0, 2);
    IPv4 ip3(192, 169, 0, 1);

    assert(ip1 < ip2);
    assert(ip2 > ip1);
    assert(ip1 <= ip2);
    assert(ip2 >= ip1);
    assert(ip1 < ip3);
}

void test_input_output() {
    IPv4 ip_out(10, 20, 30, 40);
    assert(ip_out == IPv4(10, 20, 30, 40));

    std::stringstream ss;
    ss << "172.16.0.1";

    IPv4 ip_in;
    ss >> ip_in;

    assert(ip_in == IPv4(172, 16, 0, 1));

    std::stringstream ss_bad;
    ss_bad << "256.1.1.1";

    IPv4 ip_bad;
    ss_bad >> ip_bad;

    assert(ss_bad.fail());
}

void test_edge_cases() {
    IPv4 zero(0, 0, 0, 0);
    assert(zero == IPv4(0, 0, 0, 0));

    IPv4 max(255, 255, 255, 255);
    assert(max == IPv4(255, 255, 255, 255));

    IPv4 localhost(127, 0, 0, 1);
    assert(localhost == IPv4(127, 0, 0, 1));
}

int main() {
    test_constructors();
    test_prefix_increment();
    test_postfix_increment();
    test_byte_overflow();
    test_prefix_decrement();
    test_postfix_decrement();
    test_byte_underflow();
    test_equality();
    test_comparison();
    test_input_output();
    test_edge_cases();
}
