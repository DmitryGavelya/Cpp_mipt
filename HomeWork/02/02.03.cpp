// Реализуйте алгоритм классификации символов типа char из таблицы ASCII с десятичными кодами от 32 до
// 127 включительно на пять следующих классов: заглавные буквы, строчные буквы, десятичные цифры, знаки
// препинания, прочие символы. Используйте ветвление switch с проваливанием и символьными литералами
// типа char в качестве меток в секциях case. Используйте секцию default для пятого класса. Используйте
// стандартныйсимвольныйпотоквводаstd::cin длявводасимволов.Используйтестандартныйсимвольныйпо-
// ток вывода std::cout для вывода названий классов. Не сопровождайте Ваше решение данной задачи тестами.

#include <iostream>

int main() {
  char c;
  std::cin >> c;

  switch (c) {
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
  case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
  case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
  case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
  case 'Y': case 'Z':
    std::cout << "capital letters";
    break;

  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
  case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
  case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
  case 's': case 't': case 'u': case 'v': case 'w': case 'x':
  case 'y': case 'z':
    std::cout << "lower letters";
    break;

  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
    std::cout << "numbers";
    break;

  case '!': case '"': case '#': case '$': case '%': case '&': case '\'':
  case '(': case ')': case '*': case '+': case ',': case '-': case '.':
  case '/':
  case ':': case ';': case '<': case '=': case '>': case '?': case '@':
  case '[': case '\\': case ']': case '^': case '_': case '`':
  case '{': case '|': case '}': case '~':
    std::cout << "punctuation marks";
    break;

  default:
    std::cout << "other";
    break;
  }
}
