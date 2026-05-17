#include <atomic>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <print>
#include <thread>
#include <vector>

/////////////////////////////////////////////////////////////////////////////////////

class WorkStealingQueue
{
public:

    using Task = std::function < void() >;

    void push(Task task)
    {
        std::lock_guard lock(m_mutex);
        m_deque.push_front(std::move(task));
    }

    std::optional < Task > pop()
    {
        std::lock_guard lock(m_mutex);

        if (m_deque.empty()) return std::nullopt;

        auto task = std::move(m_deque.front());
        m_deque.pop_front();
        return task;
    }

    std::optional < Task > steal()
    {
        std::lock_guard lock(m_mutex);

        if (m_deque.empty()) return std::nullopt;

        auto task = std::move(m_deque.back());
        m_deque.pop_back();
        return task;
    }

    bool empty() const
    {
        std::lock_guard lock(m_mutex);
        return m_deque.empty();
    }

private:

    mutable std::mutex       m_mutex;
    std::deque < Task >      m_deque;
};

/////////////////////////////////////////////////////////////////////////////////////

class ThreadPool
{
public:

    using Task = WorkStealingQueue::Task;

    explicit ThreadPool(std::size_t concurrency = std::thread::hardware_concurrency())
        : m_done(false)
    {
        m_queues.resize(concurrency);

        for (auto i = 0uz; i < concurrency; ++i)
        {
            m_queues[i] = std::make_unique < WorkStealingQueue >();
        }

        for (auto i = 0uz; i < concurrency; ++i)
        {
            m_threads.emplace_back([this, i] { run(i); });
        }
    }

    ~ThreadPool()
    {
        m_done.store(true);

        for (auto & t : m_threads) t.join();
    }

    template < typename F, typename ... Args >
    auto submit(F && f, Args && ... args) -> std::future < std::invoke_result_t < F, Args... > >
    {
        using R = std::invoke_result_t < F, Args... >;

        auto task = std::make_shared < std::packaged_task < R() > > (
            [f = std::forward < F >(f), ...args = std::forward < Args >(args)]() mutable
            {
                return f(std::forward < Args >(args)...);
            }
        );

        auto future = task->get_future();

        auto index = m_submit_index.fetch_add(1) % m_queues.size();

        m_queues[index]->push([task] { (*task)(); });

        return future;
    }

private:

    void run(std::size_t index)
    {
        while (!m_done.load())
        {
            if (auto task = m_queues[index]->pop())
            {
                (*task)();
                continue;
            }

            if (auto task = steal(index))
            {
                (*task)();
                continue;
            }

            std::this_thread::yield();
        }

        while (auto task = m_queues[index]->pop()) (*task)();
    }

    std::optional < Task > steal(std::size_t self)
    {
        for (auto i = 0uz; i < m_queues.size(); ++i)
        {
            if (i == self) continue;

            if (auto task = m_queues[i]->steal()) return task;
        }

        return std::nullopt;
    }

    std::atomic < bool >                                    m_done;
    std::atomic < std::size_t >                             m_submit_index { 0 };
    std::vector < std::unique_ptr < WorkStealingQueue > >   m_queues;
    std::vector < std::thread >                             m_threads;
};

/////////////////////////////////////////////////////////////////////////////////////

int main()
{
    ThreadPool pool(std::thread::hardware_concurrency());

    constexpr auto N = 1 << 14;

    std::vector < std::future < int > > futures;
    futures.reserve(N);

    for (auto i = 0; i < N; ++i)
    {
        futures.push_back(pool.submit([i] { return i * i; }));
    }

    long long sum = 0;

    for (auto & f : futures) sum += f.get();

    std::print("main : sum of squares [0..{}) = {}\n", N, sum);
}