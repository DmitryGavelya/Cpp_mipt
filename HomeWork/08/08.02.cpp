#include <cassert>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std::literals;

class Integer {
public:
    Integer() : sign_(0) {}
    Integer(long long v)               { from_long_long(v); }
    Integer(const std::string& s)      { from_string(s); }

    Integer operator+() const          { return *this; }
    Integer operator-() const          { Integer t(*this); if (t.sign_) t.sign_ = -t.sign_; return t; }

    Integer& operator+=(const Integer& rhs) {
        if (rhs.sign_ == 0) return *this;
        if (sign_ == 0) { *this = rhs; return *this; }
        if (sign_ == rhs.sign_) {
            add_abs(rhs);
        } else {
            int cmp = cmp_abs(rhs);
            if (cmp == 0) { digits_.clear(); sign_ = 0; }
            else if (cmp > 0) sub_abs(rhs);
            else {
                Integer tmp(rhs);
                tmp.sub_abs(*this);
                *this = tmp;
            }
        }
        normalize();
        return *this;
    }

    Integer& operator-=(const Integer& rhs) {
        Integer tmp(rhs);
        tmp.sign_ = -tmp.sign_;
        return *this += tmp;
    }

    Integer& operator*=(const Integer& rhs) {
        if (sign_ == 0 || rhs.sign_ == 0) {
            digits_.clear(); sign_ = 0; return *this;
        }
        std::vector<int> res(digits_.size() + rhs.digits_.size());
        for (size_t i = 0; i < digits_.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < rhs.digits_.size() || carry; ++j) {
                long long cur = res[i + j]
                              + 1LL * digits_[i] * (j < rhs.digits_.size() ? rhs.digits_[j] : 0)
                              + carry;
                res[i + j] = int(cur % 10);
                carry = int(cur / 10);
            }
        }
        digits_ = std::move(res);
        sign_ *= rhs.sign_;
        normalize();
        return *this;
    }

    Integer& operator/=(const Integer& rhs) {
        Integer q, r;
        div_mod(*this, rhs, q, r);
        *this = q;
        return *this;
    }

    friend Integer operator%(const Integer& a, const Integer& b) {
        Integer q, r;
        div_mod(a, b, q, r);
        return r;
    }

    Integer& operator%=(const Integer& b) {
        Integer q, r;
        div_mod(*this, b, q, r);
        *this = r;
        return *this;
    }

    Integer pow(unsigned int n) const {
        Integer base = *this;
        Integer res  = 1;
        while (n) {
            if (n & 1) res *= base;
            n >>= 1;
            if (n) base *= base;
        }
        return res;
    }

    int sign() const { return sign_; }

    Integer abs() const {
        Integer t(*this);
        if (t.sign_ < 0) t.sign_ = -t.sign_;
        return t;
    }

    Integer& operator++()    { *this += 1; return *this; }
    Integer  operator++(int) { Integer tmp(*this); *this += 1; return tmp; }
    Integer& operator--()    { *this -= 1; return *this; }
    Integer  operator--(int) { Integer tmp(*this); *this -= 1; return tmp; }

    friend bool operator==(const Integer& a, const Integer& b) {
        return a.sign_ == b.sign_ && a.digits_ == b.digits_;
    }
    friend bool operator!=(const Integer& a, const Integer& b) { return !(a == b); }

    friend bool operator<(const Integer& a, const Integer& b) {
        if (a.sign_ != b.sign_) return a.sign_ < b.sign_;
        if (a.sign_ == 0) return false;
        int cmp = a.cmp_abs(b);
        return a.sign_ > 0 ? (cmp < 0) : (cmp > 0);
    }
    friend bool operator>(const Integer& a, const Integer& b) { return b < a; }
    friend bool operator<=(const Integer& a, const Integer& b) { return !(b < a); }
    friend bool operator>=(const Integer& a, const Integer& b) { return !(a < b); }

    friend Integer operator+(Integer a, const Integer& b) { return a += b; }
    friend Integer operator-(Integer a, const Integer& b) { return a -= b; }
    friend Integer operator*(Integer a, const Integer& b) { return a *= b; }
    friend Integer operator/(Integer a, const Integer& b) { return a /= b; }

    friend Integer multiply(const Integer& a, const Integer& b) {
        Integer t(a);
        t *= b;
        return t;
    }

    friend std::ostream& operator<<(std::ostream& os, const Integer& x) {
        if (x.sign_ == 0) { os << '0'; return os; }
        if (x.sign_ < 0) os << '-';
        for (auto it = x.digits_.rbegin(); it != x.digits_.rend(); ++it)
            os << *it;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Integer& x) {
        std::string s;
        is >> s;
        x.from_string(s);
        return is;
    }

private:
    int sign_;
    std::vector<int> digits_;


    void from_long_long(long long v) {
        digits_.clear();
        if (v == 0) { sign_ = 0; return; }
        sign_ = (v < 0 ? -1 : 1);
        if (v < 0) v = -v;
        while (v) {
            digits_.push_back(int(v % 10));
            v /= 10;
        }
    }

    void from_string(const std::string& s) {
        digits_.clear();
        sign_ = 1;
        std::size_t i = 0;
        if (s[i] == '+') ++i;
        else if (s[i] == '-') { sign_ = -1; ++i; }
        for (; i < s.size(); ++i)
            if (std::isdigit((unsigned char)s[i]))
                digits_.push_back(s[i] - '0');
        std::reverse(digits_.begin(), digits_.end());
        normalize();
    }

    void normalize() {
        while (!digits_.empty() && digits_.back() == 0)
            digits_.pop_back();
        if (digits_.empty()) sign_ = 0;
    }

    int cmp_abs(const Integer& rhs) const {
        if (digits_.size() != rhs.digits_.size())
            return digits_.size() < rhs.digits_.size() ? -1 : 1;
        for (size_t i = digits_.size(); i-- > 0;) {
            if (digits_[i] != rhs.digits_[i])
                return digits_[i] < rhs.digits_[i] ? -1 : 1;
        }
        return 0;
    }

    void add_abs(const Integer& rhs) {
        const size_t n = std::max(digits_.size(), rhs.digits_.size());
        int carry = 0;
        if (digits_.size() < n) digits_.resize(n, 0);
        for (size_t i = 0; i < n || carry; ++i) {
            if (i == digits_.size()) digits_.push_back(0);
            int cur = digits_[i] + carry + (i < rhs.digits_.size() ? rhs.digits_[i] : 0);
            digits_[i] = cur % 10;
            carry = cur / 10;
        }
    }

    void sub_abs(const Integer& rhs) {
        int carry = 0;
        for (size_t i = 0; i < rhs.digits_.size() || carry; ++i) {
            int cur = digits_[i] - carry - (i < rhs.digits_.size() ? rhs.digits_[i] : 0);
            carry = (cur < 0);
            if (carry) cur += 10;
            digits_[i] = cur;
        }
    }

    static void div_mod(const Integer& a, const Integer& b, Integer& q, Integer& r) {
        assert(b.sign_ != 0);
        r.sign_ = 0; r.digits_.clear();
        q.sign_ = 0; q.digits_.clear();
        if (a.sign_ == 0) return;

        r.sign_ = 1;
        r.digits_.clear();
        q.digits_.assign(a.digits_.size(), 0);

        Integer bb = b.abs();
        for (size_t i = a.digits_.size(); i-- > 0;) {
            r.digits_.insert(r.digits_.begin(), a.digits_[i]);
            r.normalize();
            int x = 0, l = 0, h = 9;
            while (l <= h) {
                int m = (l + h) / 2;
                Integer t = bb;
                t *= m;
                if (t <= r) { x = m; l = m + 1; }
                else h = m - 1;
            }
            q.digits_[i] = x;
            if (x) {
                Integer t = bb;
                t *= x;
                r -= t;
            }
        }
        q.sign_ = (q.digits_.empty() ? 0 : a.sign_ * b.sign_);
        q.normalize();
        if (r.sign_ != 0) r.sign_ = a.sign_;
        r.normalize();
    }
};

int main()
{
    Integer x = std::string(32, '1');
    Integer y = std::string(32, '2');

    assert((x += y) == "+33333333333333333333333333333333"s);
    assert((x -= y) == "+11111111111111111111111111111111"s);
    assert((x *= y) == "+246913580246913580246913580246908641975308641975308641975308642"s);
    assert((x /= y) == "+11111111111111111111111111111111"s);

    assert((x++)   == "+11111111111111111111111111111111"s);
    assert((x--)   == "+11111111111111111111111111111112"s);
    assert((++y)   == "+22222222222222222222222222222223"s);
    assert((--y)   == "+22222222222222222222222222222222"s);

    assert((x +  y) == "+33333333333333333333333333333333"s);
    assert((x -  y) == "-11111111111111111111111111111111"s);
    assert((x *  y) == "+246913580246913580246913580246908641975308641975308641975308642"s);
    assert((x /  y) == "+0"s);

    assert((x <  y) == 1);
    assert((x >  y) == 0);
    assert((x <= y) == 1);
    assert((x >= y) == 0);
    assert((x == y) == 0);
    assert((x != y) == 1);

    std::stringstream stream_1(std::string(32, '1'));
    std::stringstream stream_2;

    stream_1 >> x;
    stream_2 << x;
    assert(stream_2.str() == stream_1.str());

    Integer r1 = x % y;
    Integer t  = x; t %= y;
    assert(r1 == t);

    Integer z = x.pow(2);
    assert(z == multiply(x, x));

    Integer neg = -x;
    assert(neg.abs() == x);
    assert(neg.sign() == -x.sign());

}
