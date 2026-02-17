#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>
#include <vector>
#include <deque>

////////////////////////////////////////////////////////////////////////////////////

template <typename RandomIt>
void order(RandomIt first, RandomIt last) {
    if (first == last) return;

    for (auto i = std::next(first); i != last; ++i) {
        for (auto j = i; j != first; --j) {
            auto prev = std::prev(j);
            if (*prev > *j) {
                std::iter_swap(prev, j);
            } else {
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////

template <typename RandomIt>
RandomIt partition(RandomIt first, RandomIt last) {
    auto pivot_it = std::prev(last);
    auto i = first;

    for (auto j = first; j != pivot_it; ++j) {
        if (*j <= *pivot_it) {
            std::iter_swap(i, j);
            std::advance(i, 1);
        }
    }
    std::iter_swap(i, pivot_it);
    return i;
}

template <typename RandomIt>
void quickSort(RandomIt first, RandomIt last) {
    auto dist = std::distance(first, last);

    if (dist <= 1) return;

    if (dist > 16) {
        RandomIt pi = partition(first, last);

        quickSort(first, pi);
        quickSort(std::next(pi), last);
    } else {
        order(first, last);
    }
}

template <typename RandomIt>
void sort(RandomIt first, RandomIt last) {
    quickSort(first, last);
}

////////////////////////////////////////////////////////////////////////////////////

int main() {
    auto size = 1000;

    std::vector<int> vector(size, 0);
    for (auto i = 0; i < size; ++i) {
        vector[i] = size - i;
    }

    sort(vector.begin(), vector.end());
    assert(std::ranges::is_sorted(vector));
    std::cout << "Vector sorted successfully.\n";

    std::deque<int> d(size, 0);
    for (auto i = 0; i < size; ++i) {
        d[i] = size - i;
    }

    sort(d.begin(), d.end());
    assert(std::ranges::is_sorted(d));
    std::vector<int> part_vec = {5, 4, 3, 2, 1, 0};
    sort(part_vec.begin(), part_vec.begin() + 3);

    assert(part_vec[0] == 3 && part_vec[1] == 4 && part_vec[2] == 5);
    assert(part_vec[3] == 2 && part_vec[4] == 1 && part_vec[5] == 0);
}
