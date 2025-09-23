#include <iostream>

// на семинаре

int main() {
  double array[1024] {1, 2, 3, 4, 5};
  std::size_t size = 5;
  double min = array[0];
  double max = array[0];
  double sum = array[0];

  for (std::size_t i = 1; i < size; ++i) {
    min = std::min(min, array[i]);
    max = std::max(max, array[i]);
    sum += array[i];
  }

  double avg = sum / size;
  double s = 0;

  for (std::size_t i = 0; i < size; ++i) {
    s += (array[i] - avg) * (array[i] - avg);
  }
  s = std::sqrt(s / size);

  std::cout << min << ' ' << max << ' ' << avg << ' ' << s << '\n';

}