

#include <array>
class IPv4 {
  private:
    std::array<std::uint8_t, 4> data;
  public:
    IPv4() : data{0, 0, 0, 0} {}
    IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d) : data{a, b, c, d} {}
    IPv4(const IPv4& other) = default;
    IPv4& operator=(const IPv4& other) = default;

    IPv4 operator++() {
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
          if (lhs.data[i] < rhs.data[i]) {
            return true;
          }
          if (lhs.data[i] > rhs.data[i]) {
            return false;
          }
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

        is >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d;

        if (dot1 != '.' || dot2 != '.' || dot3 != '.'
          || a < 0 || a > 255 || b < 0 || b > 255 ||
          c < 0 || c > 255 || d < 0 || d > 255) {
            throw std::invalid_argument("Некорректный формат IP адреса");
          }
          return is;

        ip.data[0] = static_cast<std::uint8_t>(a);
        ip.data[1] = static_cast<std::uint8_t>(b);
        ip.data[2] = static_cast<std::uint8_t>(c);
        ip.data[3] = static_cast<std::uint8_t>(d);

        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
      os << static_cast<int>(ip.data[0]) << "."
         << static_cast<int>(ip.data[1]) << "."
         << static_cast<int>(ip.data[2]) << "."
         << static_cast<int>(ip.data[3]) << ".";
      return os;
    }
};


int main() {}