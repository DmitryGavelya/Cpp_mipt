#include <benchmark/benchmark.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <future>
#include <numeric>
#include <ranges>
#include <thread>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

template < std::ranges::view V >
class Task
{
public :

    auto operator()(V view) const -> double
    {
        double result = 0.0;

        for (auto x : view)
        {
            result += std::sin(x) * std::cos(x) + std::sqrt(std::abs(x));
        }

        return result;
    }
};

///////////////////////////////////////////////////////////////////////////////

template < typename T >
auto fold(std::ranges::view auto view, T sum, std::size_t concurrency) -> T
{
    auto begin = std::begin(view), end = std::end(view);

    if (auto size = std::distance(begin, end); size > 0)
    {
        concurrency = std::max(concurrency, std::size_t(1));

        std::vector < std::pair < std::future < T >, std::jthread > > futures(concurrency - 1);

        auto step = size / static_cast < std::ptrdiff_t > (concurrency);

        for (auto & [future, thread] : futures)
        {
            auto range = std::ranges::subrange(begin, std::next(begin, step));

            std::packaged_task < T(decltype(range)) > task { Task < decltype(range) >() };

            future = task.get_future();

            thread = std::jthread(std::move(task), range);

            std::advance(begin, step);
        }

        auto range = std::ranges::subrange(begin, end);

        sum += Task < decltype(range) >()(range);

        for (auto & [future, thread] : futures)
        {
            sum += future.get();
        }
    }

    return sum;
}

///////////////////////////////////////////////////////////////////////////////

static const std::size_t DATA_SIZE = 1 << 22;

static std::vector < double > make_data()
{
    std::vector < double > v(DATA_SIZE);
    std::iota(v.begin(), v.end(), 1.0);
    return v;
}

static const std::vector < double > g_data = make_data();

///////////////////////////////////////////////////////////////////////////////

static void BM_fold(benchmark::State & state)
{
    auto threads = static_cast < std::size_t > (state.range(0));

    for (auto _ : state)
    {
        auto result = fold(std::views::all(g_data), 0.0, threads);

        benchmark::DoNotOptimize(result);
    }

    state.SetLabel(std::to_string(threads) + " thread(s)");
}

BENCHMARK(BM_fold)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)
                  ->Arg(std::thread::hardware_concurrency())
                  ->Unit(benchmark::kMillisecond)
                  ->UseRealTime();

///////////////////////////////////////////////////////////////////////////////

static void BM_sequential(benchmark::State & state)
{
    for (auto _ : state)
    {
        double result = 0.0;

        for (auto x : g_data)
        {
            result += std::sin(x) * std::cos(x) + std::sqrt(std::abs(x));
        }

        benchmark::DoNotOptimize(result);
    }

    state.SetLabel("sequential baseline");
}

BENCHMARK(BM_sequential)->Unit(benchmark::kMillisecond)->UseRealTime();

///////////////////////////////////////////////////////////////////////////////

BENCHMARK_MAIN();