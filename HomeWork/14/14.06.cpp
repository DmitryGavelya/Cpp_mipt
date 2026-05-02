#include <atomic>
#include <memory>
#include <optional>
#include <print>
#include <thread>
#include <vector>

/////////////////////////////////////////////////////////////////////////////////////

template < typename T >
class Stack
{
    struct Node
    {
        T                          value;
        std::shared_ptr < Node >   next;
    };

public:

    void push(T value)
    {
        auto node        = std::make_shared < Node >();
        node->value      = std::move(value);
        node->next       = m_head.load(std::memory_order_relaxed);

        while (!m_head.compare_exchange_weak(
            node->next,
            node,
            std::memory_order_release,
            std::memory_order_relaxed));
    }

    std::optional < T > pop()
    {
        auto head = m_head.load(std::memory_order_acquire);

        while (head)
        {
            if (m_head.compare_exchange_weak(
                head,
                head->next,
                std::memory_order_acquire,
                std::memory_order_relaxed))
            {
                return std::move(head->value);
            }
        }

        return std::nullopt;
    }

    bool empty() const
    {
        return m_head.load(std::memory_order_acquire) == nullptr;
    }

private:

    std::atomic < std::shared_ptr < Node > > m_head { nullptr };
};

/////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Stack < int > stack;

    constexpr auto producers = 4;
    constexpr auto items     = 1 << 10;

    std::vector < std::jthread > threads;

    for (auto i = 0; i < producers; ++i)
    {
        threads.emplace_back([&stack, i]
        {
            for (auto j = 0; j < items; ++j)
            {
                stack.push(i * items + j);
            }
        });
    }

    threads.clear();

    std::atomic < int > count = 0;

    for (auto i = 0; i < producers; ++i)
    {
        threads.emplace_back([&stack, &count]
        {
            while (!stack.empty())
            {
                if (stack.pop()) ++count;
            }
        });
    }

    threads.clear();

    std::print("main : popped {} / {} items\n", count.load(), producers * items);
}