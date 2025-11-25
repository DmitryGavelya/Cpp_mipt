#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> class addable {
public:
  friend T operator+(T lhs, const T &rhs) { return lhs += rhs; }
};

template <typename T> class subtractable {
public:
  friend T operator-(T lhs, const T &rhs) { return lhs -= rhs; }
};

template <typename T> class multipliable {
public:
  friend T operator*(T lhs, const T &rhs) { return lhs *= rhs; }
};

template <typename T> class dividable {
public:
  friend T operator/(T lhs, const T &rhs) { return lhs /= rhs; }
};

template <typename T> class incrementable {
public:
  friend T &operator++(T &t) { return t += T(1); }

  friend T operator++(T &t, int) {
    T temp(t);
    ++t;
    return temp;
  }
};

template <typename T> class decrementable {
public:
  friend T &operator--(T &t) { return t -= T(1); }

  friend T operator--(T &t, int) {
    T temp(t);
    --t;
    return temp;
  }
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Rational : public addable<Rational>,
                 public subtractable<Rational>,
                 public multipliable<Rational>,
                 public dividable<Rational>,
                 public incrementable<Rational>,
                 public decrementable<Rational> {
public:
  /* explicit */ Rational(int num = 0, int den = 1) : m_num(num), m_den(den) {
    reduce();
  }

  //  -------------------------------------------------------------------------------------------
  explicit operator double() const { return 1.0 * m_num / m_den; }

  //  -------------------------------------------------------------------------------------------
  Rational &operator+=(Rational const &other) {
    auto lcm = std::lcm(m_den, other.m_den);

    m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);

    m_den = lcm;

    reduce();

    return *this;
  }

  //  -------------------------------------------------------------------------------------------
  Rational &operator-=(Rational const &other) {
    return *this += Rational(other.m_num * -1, other.m_den);
  }

  //  -------------------------------------------------------------------------------------------
  Rational &operator*=(Rational const &other) {
    m_num *= other.m_num;
    m_den *= other.m_den;

    reduce();

    return *this;
  }

  //  -------------------------------------------------------------------------------------------
  Rational &operator/=(Rational const &other) {
    return *this *= Rational(other.m_den, other.m_num);
  }

  //  -------------------------------------------------------------------------------------------
  friend std::strong_ordering operator<=>(Rational const &lhs,
                                          Rational const &rhs) {
    return lhs.m_num * rhs.m_den <=> rhs.m_num * lhs.m_den;
  }

  friend bool operator==(Rational const &lhs, Rational const &rhs) {
    return lhs.m_num * rhs.m_den == rhs.m_num * lhs.m_den;
  }

  //  -------------------------------------------------------------------------------------------
  friend std::istream &operator>>(std::istream &stream, Rational &rational) {
    return (stream >> rational.m_num).ignore() >> rational.m_den;
  }

  //  -------------------------------------------------------------------------------------------
  friend std::ostream &operator<<(std::ostream &stream,
                                  Rational const &rational) {
    return stream << rational.m_num << '/' << rational.m_den;
  }

private:
  void reduce() {
    if (m_den < 0) {
      m_num = -m_num;
      m_den = -m_den;
    }

    auto gcd = std::gcd(m_num, m_den);

    m_num /= gcd;
    m_den /= gcd;
  }

  //  -------------------------------------------------------------------------------------------
  int m_num = 0, m_den = 1;
};

///////////////////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6) {
  return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  Rational x = 1, y(2, 1);

  //  -----------------------------------------------------------------------
  assert(equal(static_cast<double>(x), 1));

  //  -----------------------------------------------------------------------
  assert((x += y) == Rational(+3, 1));
  assert((x -= y) == Rational(+1, 1));
  assert((x *= y) == Rational(+2, 1));
  assert((x /= y) == Rational(+1, 1));

  //  -----------------------------------------------------------------------
  assert((x + y) == Rational(+3, 1));
  assert((x - y) == Rational(-1, 1));
  assert((x * y) == Rational(+2, 1));
  assert((x / y) == Rational(+1, 2));

  //  -----------------------------------------------------------------------
  Rational a(1, 1);

  assert((++a) == Rational(2, 1));
  assert((a++) == Rational(2, 1));
  assert(a == Rational(3, 1));

  assert((--a) == Rational(2, 1));
  assert((a--) == Rational(2, 1));
  assert(a == Rational(1, 1));

  //  -----------------------------------------------------------------------
  assert((x += 1) == Rational(+2, 1));
  assert((x + 1) == Rational(+3, 1));
  assert((1 + y) == Rational(+3, 1));

  //  -----------------------------------------------------------------------
  assert((x < y) == 0);
  assert((x > y) == 0);
  assert((x <= y) == 1);
  assert((x >= y) == 1);
  assert((x == y) == 1);
  assert((x != y) == 0);

  //  -----------------------------------------------------------------------
  std::stringstream stream_1("1/2");
  std::stringstream stream_2;

  //  -----------------------------------------------------------------------
  stream_1 >> x;
  stream_2 << x;

  //  -----------------------------------------------------------------------
  assert(stream_2.str() == stream_1.str());
}

///////////////////////////////////////////////////////////////////////////////////////////////