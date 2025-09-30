// Доработайте пример 02.42 таким образом, чтобы вместо алгоритма сортировки слиянием использовался ал-
// горитм быстрой сортировки. Обоснуйте временную сложность полученного гибридного алгоритма сортировки.

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////

void order(std::vector < int > & vector, std::size_t left, std::size_t right)
{
	for (auto i = left + 1; i < right; ++i)
	{
		for (auto j = i; j > left; --j)
		{
			if (vector[j - 1] > vector[j])
			{
				std::swap(vector[j], vector[j - 1]);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
std::size_t partition(std::vector<int> &vec, std::size_t low, std::size_t high) {
	int pivot = vec[high - 1];
	std::size_t i = low;

	for (std::size_t j = low; j < high - 1; ++j) {
		if (vec[j] <= pivot) {
			std::swap(vec[i], vec[j]);
			++i;
		}
	}
	std::swap(vec[i], vec[high - 1]);
	return i;
}

void quickSort(std::vector<int> &vec, std::size_t low, std::size_t high) {
	if (high - low <= 1) return;

	if (high - low > 16) {
		std::size_t pi = partition(vec, low, high);
		quickSort(vec, low, pi);
		quickSort(vec, pi + 1, high);
	} else {
		order(vec, low, high);
	}
}

void sort(std::vector<int> & vector) {
	quickSort(vector, 0, std::size(vector));
}

////////////////////////////////////////////////////////////////////////////////////

int main()
{
	// Quick sort работает в среднем за O(nlogn) в худшем случае без использования медианы медиан за O(n^2)
	// поэтому при <= 16 элементов мы используем алгоритм, который будет сортировать в сумме за константу
	auto size = 1'000uz;

//  ---------------------------------------

	std::vector < int > vector(size, 0);

//  ---------------------------------------

	for (auto i = 0uz; i < size; ++i)
	{
		vector[i] = size - i;
	}

//  ---------------------------------------

	sort(vector);
//  ---------------------------------------

	assert(std::ranges::is_sorted(vector));
}

////////////////////////////////////////////////////////////////////////////////////
