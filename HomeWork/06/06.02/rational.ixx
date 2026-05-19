export module Rational;

import <compare>;
import <iostream>;
import <numeric>;

export namespace RationalLib {

    class Rational {
    public:
        Rational(int num = 0, int den = 1);

        explicit operator double() const;

        Rational& operator+=(const Rational& other);
        Rational& operator-=(const Rational& other);
        Rational& operator*=(const Rational& other);
        Rational& operator/=(const Rational& other);

        Rational operator++(int) { auto tmp = *this; *this += 1; return tmp; }
        Rational& operator++() { *this += 1; return *this; }
        Rational operator--(int) { auto tmp = *this; *this -= 1; return tmp; }
        Rational& operator--() { *this -= 1; return *this; }

        friend Rational operator+(Rational lhs, const Rational& rhs) { return lhs += rhs; }
        friend Rational operator-(Rational lhs, const Rational& rhs) { return lhs -= rhs; }
        friend Rational operator*(Rational lhs, const Rational& rhs) { return lhs *= rhs; }
        friend Rational operator/(Rational lhs, const Rational& rhs) { return lhs /= rhs; }

        friend std::strong_ordering operator<=>(const Rational& lhs, const Rational& rhs);
        friend bool operator==(const Rational& lhs, const Rational& rhs);

        friend std::istream& operator>>(std::istream& stream, Rational& rational);
        friend std::ostream& operator<<(std::ostream& stream, const Rational& rational);

    private:
        void reduce();
        int m_num = 0;
        int m_den = 1;
    };

}