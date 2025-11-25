#include <cassert>
#include <numeric> // для std::gcd
//////////////////////////////////////////////////////////////////////////////////////////
template <int N = 0, int D = 1> struct Ratio {
    constexpr static auto num = N;
    constexpr static auto den = D;
};

template<typename R> struct Reduce {
    constexpr static auto abs_num = R::num < 0 ? -R::num : R::num;
    constexpr static auto abs_den = R::den < 0 ? -R::den : R::den;
    constexpr static auto gcd = std::gcd(abs_num, abs_den);
    constexpr static auto sign = (R::den < 0 ? -1 : 1) * (R::num < 0 ? -1 : 1);
    constexpr static auto num = sign * (abs_num / gcd);
    constexpr static auto den = abs_den / gcd;
    using type = Ratio<num, den>;
};
//////////////////////////////////////////////////////////////////////////////////////////
template<typename R1, typename R2> struct Sum {
    constexpr static auto num = R1::num * R2::den + R2::num * R1::den;
    constexpr static auto den = R1::den * R2::den;
    using type = typename Reduce<Ratio<num, den>>::type;
};

template<typename R1, typename R2> using sum = typename Sum<R1, R2>::type;
//////////////////////////////////////////////////////////////////////////////////////////
template<typename R1, typename R2> struct Mul {
    constexpr static auto num = R1::num * R2::num;
    constexpr static auto den = R1::den * R2::den;
    using type = typename Reduce<Ratio<num, den>>::type;
};
template<typename R1, typename R2> using mul = typename Mul<R1, R2>::type;
//////////////////////////////////////////////////////////////////////////////////////////
template<typename R1, typename R2> struct Sub {
    using type = typename Sum<R1, Ratio<-R2::num, R2::den>>::type;
};
template<typename R1, typename R2> using sub = typename Sub<R1, R2>::type;
//////////////////////////////////////////////////////////////////////////////////////////
template<typename R1, typename R2> struct Div {
    static_assert(R2::num != 0);
    static_assert(R2::den != 0);
    using type = typename Mul<R1, Ratio<R2::den, R2::num>>::type;
};
template<typename R1, typename R2> using ratio_div = typename Div<R1, R2>::type;
//////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename R = Ratio<1>> struct Duration {
    T x;
    constexpr Duration(T val = T()) : x(val) {}
};
//////////////////////////////////////////////////////////////////////////////////////////
template<typename T1, typename R1, typename T2, typename R2>
constexpr auto operator+(Duration<T1, R1> const& lhs, Duration<T2, R2> const& rhs) {
    using ratio_t = Ratio<1, sum<R1, R2>::den>;
    auto x = (
        lhs.x * ratio_t::den / R1::den * R1::num +
        rhs.x * ratio_t::den / R2::den * R2::num
    );
    return Duration<decltype(x), ratio_t>{x};
}
//////////////////////////////////////////////////////////////////////////////////////////
template<typename T1, typename R1, typename T2, typename R2>
constexpr auto operator-(Duration<T1, R1> const& lhs, Duration<T2, R2> const& rhs) {
    return lhs + Duration<T2, Ratio<-R2::num, R2::den>>{rhs.x};
}
//////////////////////////////////////////////////////////////////////////////////////////
int main() {
    static_assert(sum<Ratio<1,2>, Ratio<1,3>>::num == 5);
    static_assert(sum<Ratio<1,2>, Ratio<1,3>>::den == 6);
    static_assert(sub<Ratio<1,2>, Ratio<1,3>>::num == 1);
    static_assert(sub<Ratio<1,2>, Ratio<1,3>>::den == 6);
    static_assert(mul<Ratio<1,2>, Ratio<2,3>>::num == 1);
    static_assert(mul<Ratio<1,2>, Ratio<2,3>>::den == 3);
    static_assert(ratio_div<Ratio<1,2>, Ratio<2,3>>::num == 3);
    static_assert(ratio_div<Ratio<1,2>, Ratio<2,3>>::den == 4);
    static_assert(mul<Ratio<2,4>, Ratio<2,6>>::num == 1);
    static_assert(mul<Ratio<2,4>, Ratio<2,6>>::den == 6);
}