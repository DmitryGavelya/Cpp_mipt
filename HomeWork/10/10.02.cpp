#include <gtest/gtest.h>
#include <algorithm>
#include <stack>
#include <utility>
#include <stdexcept>

template <typename T>
class MinStackPair {
public:
    void push(T x) {
        m_stack.emplace(x, m_stack.empty() ? x : std::min(x, m_stack.top().second));
    }

    T top() const {
        if (m_stack.empty()) throw std::runtime_error("Stack is empty");
        return m_stack.top().first;
    }

    void pop() {
        if (!m_stack.empty()) m_stack.pop();
    }

    T min() const {
        if (m_stack.empty()) throw std::runtime_error("Stack is empty");
        return m_stack.top().second;
    }

private:
    std::stack<std::pair<T, T>> m_stack;
};

template <typename T>
class MinStackMath {
public:
    void push(T x) {
        if (m_stack.empty()) {
            m_stack.push(x);
            m_min = x;
        } else if (x < m_min) {
            m_stack.push(2 * x - m_min);
            m_min = x;
        } else {
            m_stack.push(x);
        }
    }

    T top() const {
        if (m_stack.empty()) throw std::runtime_error("Stack is empty");
        T t = m_stack.top();
        return t < m_min ? m_min : t;
    }

    void pop() {
        if (m_stack.empty()) return;
        T t = m_stack.top();
        if (t < m_min) {
            m_min = 2 * m_min - t;
        }
        m_stack.pop();
    }

    T min() const {
        if (m_stack.empty()) throw std::runtime_error("Stack is empty");
        return m_min;
    }

private:
    std::stack<T> m_stack;
    T m_min = T();
};

TEST(MinStackTest, PairImplementation) {
    MinStackPair<int> stack;

    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.min(), 3);

    stack.push(1);
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.min(), 1);

    stack.push(2);
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.min(), 1);

    stack.pop();
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.min(), 1);

    stack.pop();
    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.min(), 3);

    stack.pop();
}

TEST(MinStackTest, MathImplementation) {
    MinStackMath<int> stack;

    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.min(), 3);

    stack.push(1);
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.min(), 1);

    stack.push(2);
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.min(), 1);

    stack.pop();
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.min(), 1);

    stack.pop();
    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.min(), 3);

    stack.pop();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
