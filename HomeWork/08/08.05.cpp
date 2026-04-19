#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

template <typename D = std::chrono::duration<double>>
class Timer {
public:
    using clock_t    = std::chrono::steady_clock;
    using duration_t = D;

    explicit Timer(std::string scope)
        : m_scope(std::move(scope)) {}

    void start() {
        if (m_running) return;
        m_begin   = clock_t::now();
        m_running = true;
    }

    void stop() {
        if (!m_running) return;
        auto end = clock_t::now();
        m_intervals.push_back(
            std::chrono::duration_cast<D>(end - m_begin));
        m_running = false;
    }

    bool running() const noexcept { return m_running; }

    const std::vector<D>& intervals() const noexcept {
        return m_intervals;
    }

    double average() const {
        if (m_intervals.empty()) return 0.0;
        D sum{0};
        for (auto const& d : m_intervals) sum += d;
        D avg = sum / static_cast<double>(m_intervals.size());
        return avg.count();
    }

    ~Timer() {
        if (!m_scope.empty() && !m_intervals.empty()) {
            std::cout << m_scope << " : "
                      << std::fixed << std::setprecision(6)
                      << average() << " s (avg over "
                      << m_intervals.size() << " runs)\n";
        }
    }

private:
    std::string           m_scope;
    clock_t::time_point   m_begin{};
    bool                  m_running{false};
    std::vector<D>        m_intervals;
};

double calculate(std::size_t size) {
    double x = 0.0;
    for (std::size_t i = 0; i < size; ++i) {
        x += std::pow(std::sin(static_cast<double>(i)), 2)
           + std::pow(std::cos(static_cast<double>(i)), 2);
    }
    return x;
}

bool equal(double x, double y, double eps = 1e-6) {
    return std::abs(x - y) < eps;
}

int main() {
    Timer<> timer("main : timer");

    const std::size_t runs = 5;
    for (std::size_t i = 0; i < runs; ++i) {
        timer.start();
        double res = calculate(1'000'000);
        timer.stop();

        assert(equal(res, 1'000'000));
    }
}
