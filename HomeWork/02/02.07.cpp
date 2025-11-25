#include <iostream>
#include <algorithm>
#include <cmath>

int main() {
  int size;
  std::cin >> size;

  if (size <= 0) {
    return 0;
  }

  double* array = new double[size];

  for (int i = 0; i < size; ++i) {
    std::cin >> array[i];
  }

  double max = array[0];
  double min = array[0];
  double sum = 0.0;

  for (int i = 0; i < size; ++i) {
    max = std::max(max, array[i]);
    min = std::min(min, array[i]);
    sum += array[i];
  }

  double avg = sum / static_cast<double>(size);
  double s = 0.0;

  for (int i = 0; i < size; ++i) {
    double diff = array[i] - avg;
    s += diff * diff;
  }
  s = std::sqrt(s / static_cast<double>(size));

  std::cout << "max: " << max << '\n'
            << "min: " << min << '\n'
            << "average: " << avg << '\n'
            << "deviation: " << s << '\n';

  delete[] array;
  return 0;
}
