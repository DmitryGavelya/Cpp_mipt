// Реализуйте внешнюю систему тестирования приватных функций-членов некоторого класса. Реализуйте класс
// Entity. Реализуйте в классе Entity две демонстрационные приватные функции-члены, которые необходимо
// будет подвергнуть тестированию. Вспомните о принципе SOLID единственности ответственности. Реализуй-
// те два дополнительных самостоятельных класса-тестировщика, каждый из которых будет предназначен для
// тестирования одной соответствующей ему приватной функции-члена класса Entity. Используйте отноше-
// ние дружбы между классами-тестировщиками и классом Entity, чтобы выполнить тестирование приватных
// функций-членов класса Entity без необходимости обращения к его публичному интерфейсу. Используйте
// паттерн Attorney-Client для ограничения доступа классов-тестировщиков к приватной секции класса Entity


class Entity {
  private:
    int secret_code1 = 42;
    int secret_code2 = 43;

    bool secret_fun_1(int number) {
      return number == secret_code1;
    }

    bool secret_fun_2(int number) {
        return number == secret_code2;
    }
  friend class Attorney_Client;
};

class Attorney_Client {
  private:
  static bool call_private_method_func_1(Entity& entity, int number) {
    return entity.secret_fun_1(number);
  }
  static bool call_private_method_func_2(Entity& entity, int number) {
    return entity.secret_fun_2(number);
  }

  friend class Test1;
  friend class Test2;
};

class Test1 {
  public:
    static bool test(Entity& entity, int input, int expected) {
      int result = Attorney_Client::call_private_method_func_1(entity, input);
      return result == expected;
    }
};

class Test2 {
  public:
    static bool test(Entity& entity, int input, int expected) {
      int result = Attorney_Client::call_private_method_func_2(entity, input);
      return result == expected;
    }
};


int main() {

}