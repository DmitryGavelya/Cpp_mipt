#include <benchmark/benchmark.h>
#include <cmath>
#include <future>
#include <print>
#include <random>
#include <ranges>
#include <thread>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

class Task
{
public :

    auto operator()(std::size_t samples, std::uint64_t seed) const -> std::size_t
    {
        std::mt19937_64 engine(seed);

        std::uniform_real_distribution < double > distribution(0.0, 1.0);

        std::size_t inside = 0;

        for (std::size_t i = 0; i < samples; ++i)
        {
            auto x = distribution(engine);
            auto y = distribution(engine);

            if (x * x + y * y <= 1.0)
            {
                ++inside;
            }
        }

        return inside;
    }
};

///////////////////////////////////////////////////////////////////////////////

auto monte_carlo_pi(std::size_t total_samples, std::size_t concurrency) -> double
{
    concurrency = std::max(concurrency, std::size_t(1));

    auto samples_per_thread = total_samples / concurrency;

    std::vector < std::pair < std::future < std::size_t >, std::jthread > > futures(concurrency - 1);

    std::random_device rd;

    for (auto i = 0uz; auto & [future, thread] : futures)
    {
        std::packaged_task < std::size_t(std::size_t, std::uint64_t) > task { Task() };

        future = task.get_future();

        thread = std::jthread(std::move(task), samples_per_thread, rd() ^ (i++ * 0xdeadbeef));
    }

    std::size_t inside = Task()(samples_per_thread, rd());

    for (auto & [future, thread] : futures)
    {
        inside += future.get();
    }

    auto actual_samples = samples_per_thread * concurrency;

    return 4.0 * static_cast < double > (inside) / static_cast < double > (actual_samples);
}

///////////////////////////////////////////////////////////////////////////////

int main()
{
    constexpr std::size_t samples     = 1ull << 26;
    auto                  concurrency = std::thread::hardware_concurrency();

    auto pi = monte_carlo_pi(samples, concurrency);

    std::print("monte_carlo_pi : samples = {} | threads = {} | pi ≈ {:.8f} | error = {:.2e}\n",
        samples, concurrency, pi, std::abs(pi - std::numbers::pi));
}

///////////////////////////////////////////////////////////////////////////////

static void BM_monte_carlo(benchmark::State & state)
{
    constexpr std::size_t samples = 1ull << 24;

    auto threads = static_cast < std::size_t > (state.range(0));

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(monte_carlo_pi(samples, threads));
    }

    state.SetLabel(std::to_string(threads) + " thread(s)");
}

BENCHMARK(BM_monte_carlo)
    ->Arg(1)
    ->Arg(2)
    ->Arg(4)
    ->Arg(8)
    ->Arg(std::thread::hardware_concurrency())
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK_MAIN();