module Rational;

import <numeric>;
import <iostream>;

namespace RationalLib {

Rational::Rational(int num, int den) : m_num(num), m_den(den) {
    reduce();
}

Rational::operator double() const {
    return static_cast<double>(m_num) / m_den;
}

Rational& Rational::operator+=(const Rational& other) {
    auto lcm = std::lcm(m_den, other.m_den);
    m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
    m_den = lcm;
    reduce();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    return *this += Rational(-other.m_num, other.m_den);
}

Rational& Rational::operator*=(const Rational& other) {
    m_num *= other.m_num;
    m_den *= other.m_den;
    reduce();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    return *this *= Rational(other.m_den, other.m_num);
}

std::strong_ordering operator<=>(const Rational& lhs, const Rational& rhs) {
    return lhs.m_num * rhs.m_den <=> rhs.m_num * lhs.m_den;
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.m_num * rhs.m_den == rhs.m_num * lhs.m_den;
}

std::istream& operator>>(std::istream& stream, Rational& rational) {
    return (stream >> rational.m_num).ignore() >> rational.m_den;
}

std::ostream& operator<<(std::ostream& stream, const Rational& rational) {
    return stream << rational.m_num << '/' << rational.m_den;
}

void Rational::reduce() {
    if (m_den < 0) {
        m_num = -m_num;
        m_den = -m_den;
    }
    auto gcd = std::gcd(m_num, m_den);
    m_num /= gcd;
    m_den /= gcd;
}

}