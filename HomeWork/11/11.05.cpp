#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <random>
#include <numeric>
#include <cmath>

namespace rng = std::ranges;
namespace vws = std::views;

template <rng::input_range R, typename Out, typename Pred, typename Op>
Out transform_if(R&& r, Out out, Pred pred, Op op) {
    auto filtered = r | vws::filter(pred);
    auto transformed = filtered | vws::transform(op);
    return rng::copy(transformed, out).out;
}

template <rng::input_range R1, rng::input_range R2>
double calculate_mae(R1&& r1, R2&& r2) {
    auto diffs = vws::zip_transform([](double a, double b) { return std::abs(a - b); }, r1, r2);
    double sum = std::reduce(diffs.begin(), diffs.end(), 0.0);
    return sum / rng::distance(r1);
}

template <rng::input_range R1, rng::input_range R2>
double calculate_mse(R1&& r1, R2&& r2) {
    auto sq_diffs = vws::zip_transform([](double a, double b) { return (a - b) * (a - b); }, r1, r2);
    double sum = std::reduce(sq_diffs.begin(), sq_diffs.end(), 0.0);
    return sum / rng::distance(r1);
}

class Fibonacci : public std::ranges::view_interface<Fibonacci> {
private:
    class Iterator {
    private:
        int a = 0;
        int b = 1;
        int count = 0;
        int max_count;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        Iterator() : max_count(0) {}
        explicit Iterator(int max) : max_count(max) {}

        int operator*() const { return a; }

        Iterator& operator++() {
            int next = a + b;
            a = b;
            b = next;
            count++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return count == other.count;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    int n;

public:
    explicit Fibonacci(int n) : n(n) {}

    Iterator begin() const {
        return Iterator(n);
    }

    Iterator end() const {
        Iterator it(n);
        for(int i=0; i<n; ++i) ++it;
        return it;
    }
};


int main() {
    std::vector<int> v1 = {1, 2, 2, 3, 4, 5};
    rng::replace(v1, 2, 9);

    std::vector<int> v2(5);
    rng::fill(v2, 7);

    std::vector<int> v3 = {1, 1, 2, 2, 3};
    auto ret = rng::unique(v3);
    v3.erase(ret.begin(), ret.end());

    std::vector<int> v4 = {1, 2, 3, 4, 5};
    rng::rotate(v4, v4.begin() + 2);

    std::vector<int> v5 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> v_sampled;
    std::mt19937 gen{std::random_device{}()};
    rng::sample(v5, std::back_inserter(v_sampled), 3, gen);

    std::vector<int> v_transform_in = {1, 2, 3, 4, 5};
    std::vector<int> v_transform_out;
    transform_if(v_transform_in, std::back_inserter(v_transform_out),
                 [](int x) { return x % 2 != 0; },
                 [](int x) { return x * 2; });

    std::vector<double> y_true = {3.0, -0.5, 2.0, 7.0};
    std::vector<double> y_pred = {2.5, 0.0, 2.0, 8.0};
    double mae = calculate_mae(y_true, y_pred);
    double mse = calculate_mse(y_true, y_pred);

    std::vector<int> v_views = {1, 2, 3, 4, 5, 6};
    auto filtered_view = v_views | vws::filter([](int x) { return x % 2 == 0; });
    auto dropped_view = v_views | vws::drop(2);

    std::vector<std::vector<int>> v_join_in = {{1, 2}, {3, 4}};
    auto joined_view = v_join_in | vws::join;

    std::vector<int> v_zip1 = {1, 2, 3};
    std::vector<char> v_zip2 = {'a', 'b', 'c'};
    auto zipped_view = vws::zip(v_zip1, v_zip2);

    auto strided_view = v_views | vws::stride(2);

    Fibonacci fib(10);
    for (int x : fib) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}
