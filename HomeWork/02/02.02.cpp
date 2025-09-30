// Реализуйте алгоритм вычисления корней алгебраического уравнения второй степени с коэффициентами a, b
// и c типа double. Используйте ветвления if для проверки значения коэффициента a и значения дискриминан-
// та. Используйте константу epsilon и стандартную функцию std::abs для корректного сравнения чисел типа
// double с заданной точностью. Допускайте появление отрицательного нуля. Используйте стандартный сим-
// вольный поток ввода std::cin для ввода коэффициентов a, b и c. Используйте стандартныйсимвольныйпо-
// токвыводаstd::cout длявыводакорнейуравнения.НесопровождайтеВашерешениеданнойзадачитестами.

#include <iostream>
#include <cmath>

int main()
{
  double a = 0.0;
  double b = 0.0;
  double c = 0.0;
  const double epsilon = 1e-6;

  std::cin >> a >> b >> c;

  if (std::abs(a) < epsilon)
  {
    if (std::abs(b) < epsilon)
    {
      if (std::abs(c) < epsilon)
      {
        std::cout << "any x\n";
      }
      else
      {
        std::cout << "no solutions\n";
      }
    }
    else
    {
      std::cout << -c / b << '\n';
    }
  }
  else
  {
    double discriminant = b * b - 4.0 * a * c;
    if (discriminant < -epsilon)
    {
      std::cout << "no real solutions\n";
    }
    else if (std::abs(discriminant) < epsilon)
    {
      std::cout << -b / (2.0 * a) << '\n';
    }
    else
    {
      double sqrt_d = std::sqrt(discriminant);
      double first_sol = (-b + sqrt_d) / (2.0 * a);
      double second_sol = (-b - sqrt_d) / (2.0 * a);
      std::cout << first_sol << '\n' << second_sol << '\n';
    }
  }
}