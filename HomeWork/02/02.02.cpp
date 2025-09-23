// Реализуйте алгоритм вычисления корней алгебраического уравнения второй степени с коэффициентами a, b
// и c типа double. Используйте ветвления if для проверки значения коэффициента a и значения дискриминан-
// та. Используйте константу epsilon и стандартную функцию std::abs для корректного сравнения чисел типа
// double с заданной точностью. Допускайте появление отрицательного нуля. Используйте стандартный сим-
// вольный поток ввода std::cin для ввода коэффициентов a, b и c. Используйте стандартныйсимвольныйпо-
// токвыводаstd::cout длявыводакорнейуравнения.НесопровождайтеВашерешениеданнойзадачитестами.


#include <iostream>

int main()
{
  double a = 0;
  double b = 0;
  double c = 0;
  double const epsilon = 1e-6;

  std::cin >> a >> b >> c;

  if (std::abs(a) < epsilon)
  {
   if (std::abs(b) < epsilon)
   {
     if (std::abs(c) < epsilon)
     {
       std::cout << "any x" << '\n';
     } else
       {
       std::cout << "no solutions" << '\n';
       }
   } else
     {
     std::cout << 1.0 * -c / b << '\n';
     }
  } else
    {
    double determinant = b * b - 4 * a * c;
    if (determinant < -epsilon) {
      std::cout << "not real solutions" << '\n';
    } else
      {
      if (std::abs(determinant) < epsilon)
      {
        std::cout << "solution: " << -b / 2.0 * a << '\n';
      } else
        {
        double first_sol = (-b + std::sqrt(determinant)) / 2 * a;
        double second_sol = (-b - std::sqrt(determinant)) / 2 * a;
        std::cout << "first sol: " << first_sol << '\n' << "second sol: " << second_sol << '\n';
        }
      }
    }
}
