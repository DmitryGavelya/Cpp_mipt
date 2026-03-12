#include <gtest/gtest.h>
#include <vector>
#include <deque>
#include <iostream>

TEST(VectorTest, CapacityMultiplicationFactor) {
  std::vector<int> v;
  size_t prev_capacity = v.capacity();

  std::cout << "Vector Capacity Tracking:\n";
  for (int i = 0; i < 100; ++i) {
    v.push_back(i);
    if (v.capacity() != prev_capacity) {
      std::cout << "Size: " << v.size()
                << "\tCapacity: " << v.capacity()
                << "\tFactor: ";
      if (prev_capacity > 0) {
        std::cout << static_cast<double>(v.capacity()) / static_cast<double>(prev_capacity);
      } else {
        std::cout << "N/A";
      }
      std::cout << "\n";
      prev_capacity = v.capacity();
    }
  }
}

TEST(DequeTest, PageSizeDetermination) {
  std::deque<int> d;
  for (int i = 0; i < 2000; ++i) {
    d.push_back(i);
  }

  size_t max_streak = 0;
  size_t streak = 1;

  std::cout << "Deque Address Tracking:\n";
  for (size_t i = 0; i < d.size() - 1; ++i) {
    if (&d[i + 1] == &d[i] + 1) {
      streak++;
    } else {
      if (streak > max_streak) {
        max_streak = streak;
      }
      std::cout << "Jump at index " << i
                << "\tFrom: " << static_cast<const void*>(&d[i])
                << "\tTo: " << static_cast<const void*>(&d[i + 1])
                << "\tStreak: " << streak << "\n";
      streak = 1;
    }
  }
  if (streak > max_streak) {
    max_streak = streak;
  }

  std::cout << "Elements per page: " << max_streak << "\n";
  std::cout << "Page size in bytes: " << max_streak * sizeof(int) << "\n";
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
