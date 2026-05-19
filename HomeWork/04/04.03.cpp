#include <iostream>
#include <vector>

template <typename Container>
void handle(Container& c, int value) {
    c.push_back(value);
}

template <typename Container, typename T>
void handle(Container& c, T&& value) {}

template <typename Container, typename... Args>
void insert_ints(Container& c, Args&&... args) {
    (handle(c, std::forward<Args>(args)), ...);
}

int main() {
    std::vector<int> v;
    
    insert_ints(v, 10, "hello", 20.5, 20, 'a', 30);
    
    std::cout << "Vector content: ";
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    if (v.size() == 3 && v[0] == 10 && v[1] == 20 && v[2] == 30) {
        std::cout << "Test passed\n";
    } else {
        std::cout << "Test failed\n";
    }
}
