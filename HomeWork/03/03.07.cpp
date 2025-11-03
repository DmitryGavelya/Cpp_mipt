// Доработайте пример 03.30 таким образом, чтобы вектор мог увеличивать емкость встроенного динамического
// массива при добавлении новых элементов. Реализуйте в классе Vector два приватных поля типа std::size_t
// для хранения значений емкости и размера вектора. Реализуйте в классе Vector публичные функции-члены
// capacity и size для получения значений емкости и размера вектора соответственно. Реализуйте в классе
// Vector публичную функцию-член push_back для добавления нового элемента в первую свободную ячейку
// памяти вектора с возможностью увеличения емкости встроенного динамического массива в случае нехватки
// свободных ячеек памяти. Используйте алгоритм увеличения емкости встроенного динамического массива из
// Вашего предыдущего решения задачи 02.07. Реализуйте в классе Vector публичную функцию-член clear для
// удаления всех элементов вектора без выполнения освобождения выделенных для них ячеек памяти. Реализуй-
// те в классе Vector публичную функцию-член empty для проверки наличия хотя бы одного элемента в векторе.

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <utility>

////////////////////////////////////////////////////////////////////////////////////

class Vector
{
public:
    Vector() : m_array(nullptr), m_size(0), m_capacity(0)
    {
        std::print("Vector:: Vector (1)\n");
    }

//  --------------------------------------------------------------------------------

    Vector(std::initializer_list<int> list) : m_size(std::size(list)), m_capacity(std::size(list))
    {
        m_array = m_capacity ? new int[m_capacity]{} : nullptr;
        std::ranges::copy(list, m_array);
    }

//  --------------------------------------------------------------------------------

    Vector(Vector const& other) : m_size(other.m_size), m_capacity(other.m_capacity)
    {

        m_array = m_capacity ? new int[m_capacity]{} : nullptr;
        std::ranges::copy(other.m_array, other.m_array + other.m_size, m_array);
    }

//  --------------------------------------------------------------------------------

    Vector(Vector&& other)
        : m_array(std::exchange(other.m_array, nullptr))
        , m_size(std::exchange(other.m_size, 0))
        , m_capacity(std::exchange(other.m_capacity, 0))
    {}

//  --------------------------------------------------------------------------------

    ~Vector()
    {
        std::print("Vector::~Vector\n");
        delete[] m_array;
    }

//  --------------------------------------------------------------------------------

    auto& operator=(Vector other)
    {
        std::print("Vector::operator= (4)\n");
        swap(other);
        return *this;
    }

//  --------------------------------------------------------------------------------

    std::size_t size() const { return m_size; }
    std::size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }

//  --------------------------------------------------------------------------------

    void push_back(int value)
    {
        // Если места нет, увеличиваем емкость
        if (m_size >= m_capacity)
        {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }

        m_array[m_size] = value;
        ++m_size;
    }

//  --------------------------------------------------------------------------------

    void clear()
    {
        // Удаляем все элементы, но не освобождаем память
        m_size = 0;
    }

//  --------------------------------------------------------------------------------

    void reserve(std::size_t new_capacity)
    {
        if (new_capacity <= m_capacity)
            return;

        int* new_array = new int[new_capacity] {};

        if (m_array)
        {
            std::ranges::copy(m_array, m_array + m_size, new_array);
            delete[] m_array;
        }

        m_array = new_array;
        m_capacity = new_capacity;
    }

//  --------------------------------------------------------------------------------

    void swap(Vector& other)
    {
        std::swap(m_array, other.m_array);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

private:
    int* m_array = nullptr;
    std::size_t m_size = 0;
    std::size_t m_capacity = 0;
};

////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Vector vec;
    assert(vec.size() == 0);
    assert(vec.capacity() == 0);
    assert(vec.empty());

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    assert(vec.size() == 3);
    assert(vec.capacity() >= 3);
    assert(!vec.empty());

    vec.clear();

    assert(vec.size() == 0);
    assert(vec.capacity() >= 3);
    assert(vec.empty());


    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
        assert(vec.size() == static_cast<std::size_t>(i + 1));
        assert(vec.capacity() >= vec.size());
        assert(!vec.empty());

    }

    assert(vec.size() == 10);
    assert(vec.capacity() >= 10);
    assert(!vec.empty());
}