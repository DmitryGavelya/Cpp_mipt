#include <iterator>
#include <memory>
#include <iostream>
#include <cassert>

template <typename T> class List {
private:
    struct Node {
        T x = T();
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(T val) : x(val) {}
    };

public:
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(std::shared_ptr<Node> node = nullptr) : m_node(node) {}


        auto const operator++(int) {
            auto temp = *this;
            if (m_node) m_node = m_node->next;
            return temp;
        }

        auto& operator++() {
            if (m_node) m_node = m_node->next;
            return *this;
        }


        auto const operator--(int) {
            auto temp = *this;
            if (m_node) {
                m_node = m_node->prev.lock();
            } else {
            }
            return temp;
        }

        auto& operator--() {
            if (m_node) {
                m_node = m_node->prev.lock();
            }
            return *this;
        }

        auto& operator*() const { return m_node->x; }

        auto operator->() const { return &m_node->x; }

        friend bool operator==(Iterator const& lhs, Iterator const& rhs) {
            return lhs.m_node == rhs.m_node;
        }

        friend bool operator!=(Iterator const& lhs, Iterator const& rhs) {
            return lhs.m_node != rhs.m_node;
        }

    private:
        std::shared_ptr<Node> m_node;
    };

    auto begin() const { return Iterator(m_head); }

    auto end() const { return Iterator(nullptr); }

    auto last() const { return Iterator(m_tail); }

    //  -----------------------------------------------------------------------

    void push_back(T x) {
        auto node = std::make_shared<Node>(x);

        if (m_tail) {
            m_tail->next = node;
            node->prev = m_tail;
            m_tail = node;
        } else {
            m_head = node;
            m_tail = node;
        }
    }

private:
    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail;
};

///////////////////////////////////////////////////////////////////////////////////

int main() {
    List<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    auto it = list.last();
    assert(*it == 3);
    --it;
    assert(*it == 2);
    --it;
    assert(*it == 1);
}
