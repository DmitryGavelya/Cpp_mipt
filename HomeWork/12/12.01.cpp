#include <iostream>
#include <locale>
#include <sstream>
#include <iomanip>
#include <string>

int main() {
  try {
    std::locale ru_loc("ru_RU.utf8");
    std::locale us_loc("en_US.utf8");

    std::stringstream ss_in;
    ss_in.imbue(ru_loc);

    std::string input_money;
    std::cout << "Введите сумму в RUB: ";
    std::getline(std::cin, input_money);

    ss_in.str(input_money);

    long double rub_val = 0.0;

    ss_in >> std::get_money(rub_val, true);

    if (ss_in.fail()) {
      std::cerr << "Ошибка: Не удалось распознать введенную сумму.\n";
      return 1;
    }

    long double conversion_rate = 92.5;

    long double usd_val = rub_val / conversion_rate;

    std::stringstream ss_out;
    ss_out.imbue(us_loc);

    ss_out << std::showbase << std::put_money(usd_val, true);

    std::cout << "Сумма в USD: " << ss_out.str() << "\n";

  } catch (const std::runtime_error& e) {
    std::cerr << "Ошибка локали: Убедитесь, что локали ru_RU.utf8 и en_US.utf8 установлены в системе.\n";
  }
}
