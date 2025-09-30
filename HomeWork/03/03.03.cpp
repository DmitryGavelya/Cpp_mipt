// Реализуйте структуру Node для представления узлов односвязного списка. Реализуйте в структуре Node поле
// типа int для хранения значения текущего узла списка и поле типа Node * для хранения адреса следующего
// узла списка. Реализуйте класс List для представления односвязного списка. Реализуйте в классе List два
// приватных поля типа Node * для хранения адресов головного и хвостового узлов списка. Не создавайте в
// классе List поле для хранения текущей длины списка. Реализуйте в классе List публичную функцию-член
// empty для проверки наличия узлов в списке. Реализуйте в классе List публичную функцию-член show для
// вывода в стандартный символьный поток вывода std::cout значений всех текущих узлов списка. Реализуйте
// в классе List публичные функции-члены push_front и push_back для добавления новых узлов в начало и
// в конец списка соответственно. Используйте оператор new для динамического выделения памяти. Реализуйте
// в клас се List публичные функции-члены pop_front и pop_back для удаления узлов из начала и из конца
// списка соответственно. Используйте оператор delete для освобождения памяти. Реализуйте в классе List
// публичную функцию-член get для получения значения текущего среднего узла списка. Используйте только
// один цикл для обхода списка в функции-члене get. Реализуйте деструктор, который корректно освободит па-
// мять,выделеннуюприсозданииузловсписка.Используйтелитералnullptr длясозданиянулевыхуказателей.


#include <iostream>
#include <stdexcept>

// Структура узла списка
struct Node {
    int value;
    Node* next;

    Node(int v = 0, Node* n = nullptr) : value(v), next(n) {}
};

class List {
private:
    Node* head;
    Node* tail;

public:
    List() : head(nullptr), tail(nullptr) {}

    ~List() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    bool empty() const {
        return head == nullptr;
    }

    void show() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value;
            if (current->next != nullptr) {
                std::cout << ' ';
            }
            current = current->next;
        }
        std::cout << '\n';
    }

    // Добавление в начало
    void push_front(int value) {
        Node* new_node = new Node(value, head);
        head = new_node;
        if (tail == nullptr) {
            tail = head;
        }
    }

    // Добавление в конец
    void push_back(int value) {
        Node* new_node = new Node(value, nullptr);
        if (tail == nullptr) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    // Удаление из начала
    void pop_front() {
        if (head == nullptr) {
            return;
        }

        Node* temp = head;
        head = head->next;

        if (head == nullptr) {
            tail = nullptr;
        }

        delete temp;
    }

    // Удаление из конца
    void pop_back() {
        if (head == nullptr) {
            return;
        }

        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }

        // Находим предпоследний узел
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }

        delete tail;
        tail = current;
        tail->next = nullptr;
    }

    // Получение значения среднего узла
    int get() const {
        if (head == nullptr) {
            throw std::runtime_error("get() on empty list");
        }

        Node* slow = head;
        Node* fast = head;

        // Алгоритм черепахи и зайца
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow->value;
    }

    // Запрещаем копирование и присваивание
    List(const List&) = delete;
    List& operator=(const List&) = delete;
};

// Пример использования
int main() {
    List list;

    // Тестирование push_back
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    std::cout << "List after push_back 1-5: ";
    list.show();
    std::cout << "Middle value: " << list.get() << '\n';

    // Тестирование push_front
    list.push_front(0);
    std::cout << "After push_front(0): ";
    list.show();
    std::cout << "Middle value: " << list.get() << '\n';

    // Тестирование pop_back
    list.pop_back();
    std::cout << "After pop_back(): ";
    list.show();

    // Тестирование pop_front
    list.pop_front();
    std::cout << "After pop_front(): ";
    list.show();

    // Тестирование empty
    List empty_list;
    std::cout << "Empty list is empty: " << std::boolalpha << empty_list.empty() << '\n';
    std::cout << "Our list is empty: " << std::boolalpha << list.empty() << '\n';

    // Тестирование исключения
    try {
        empty_list.get();
    } catch (const std::runtime_error& e) {
        std::cout << "Caught exception: " << e.what() << '\n';
    }

    return 0;
}