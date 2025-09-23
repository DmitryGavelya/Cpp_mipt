// Реализуйте алгоритм классификации символов типа char из таблицы ASCII с десятичными кодами от 32 до
// 127 включительно на пять следующих классов: заглавные буквы, строчные буквы, десятичные цифры, знаки
// препинания, прочие символы. Используйте ветвление switch с проваливанием и символьными литералами
// типа char в качестве меток в секциях case. Используйте секцию default для пятого класса. Используйте
// стандартныйсимвольныйпотоквводаstd::cin длявводасимволов.Используйтестандартныйсимвольныйпо-
// ток вывода std::cout для вывода названий классов. Не сопровождайте Ваше решение данной задачи тестами.

#include <iostream>

int main() {
  int n;
  std::cin >> n;
  switch (auto y = n; y) {
  case 65: case 66: case 67: case 68: case 69: case 70:
  case 71: case 72: case 73: case 74: case 75: case 76:
  case 77: case 78: case 79: case 80: case 81: case 82:
  case 83: case 84: case 85: case 86: case 87: case 88:
  case 89: case 90:
    std::cout << "capital letters";
    break;

  case 97: case 98: case 99: case 100: case 101: case 102:
  case 103: case 104: case 105: case 106: case 107: case 108:
  case 109: case 110: case 111: case 112: case 113: case 114:
  case 115: case 116: case 117: case 118: case 119: case 120:
  case 121: case 122:
    std::cout << "lower letters";
    break;

  case 49: case 50: case 51: case 52: case 53:
  case 54: case 55: case 56: case 57:
    std::cout << "numbers";
    break;

  case 33: case 34: case 35: case 36: case 37: case 38:
  case 39: case 40: case 41: case 42: case 43: case 44:
  case 45: case 46: case 47:
    std::cout << "punctuation marks";
    break;

  default:
    std::cout << "other digital";
    break;
  }
}
