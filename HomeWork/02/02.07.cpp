#include <iostream>
#include <vector>

int main() {
  int size;
  std::cin >> size;
  std::vector<double> v(size);

  for (std::size_t i = 0; i < size; ++i) {
    std::cin >> v[i];
  }

  double max = v[0];
  double min = v[0];
  double sum = 0.0;

  for (std::size_t i = 0; i < size; ++i) {
    max = std::max(max, v[i]);
    min = std::min(min, v[i]);
    sum += v[i];
  }

  double avg = sum / size;
  double s = 0;

  for (std::size_t i = 0; i < size; ++i) {
    s += (v[i] - avg) * (v[i] - avg);
  }
  s = std::sqrt(s / size);

  std::cout << "max: " << max << '\n' << "min: " << min << '\n' << "average: " << avg << '\n' << "deviation: " << s << '\n';

}