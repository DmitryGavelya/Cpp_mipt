#include <iostream>
#include <cassert>
#include <optional>
#include <variant>
#include <utility>
#include <cmath>

using SolutionVariant = std::variant<
    std::monostate,
    double,
    std::pair<double, double>
>;

SolutionVariant solve(double a, double b, double c)
{
    double const epsilon = 1e-6;

    if (std::abs(a) < epsilon)
    {
        if (std::abs(b) < epsilon)
        {
            if (std::abs(c) < epsilon)
            {
                return std::monostate{};
            }
            else
            {
                return std::monostate{};
            }
        }
        else
        {
            return -c / b;
        }
    }

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < -epsilon)
    {
        return std::monostate{};
    }
    else if (std::abs(discriminant) < epsilon)
    {
        return -b / (2.0 * a);
    }
    else
    {
        double sqrt_disc = std::sqrt(discriminant);
        double root1 = (-b + sqrt_disc) / (2.0 * a);
        double root2 = (-b - sqrt_disc) / (2.0 * a);
        return std::make_pair(root1, root2);
    }
}

int main()
{
    double epsilon = 1e-6;

    auto result2 = solve(1.0, -2.0, 1.0);
    assert(std::get_if<double>(&result2) != nullptr);
    double root = std::get<double>(result2);
    assert(std::abs(root - 1.0) < epsilon);
    std::cout << "Test 1 (one root): passed\n";

    auto result3 = solve(1.0, 0.0, 1.0);
    assert(std::get_if<std::monostate>(&result3) != nullptr);
    std::cout << "Test 2 (no real solutions): passed\n";

    auto result4 = solve(0.0, 2.0, -4.0);
    assert(std::get_if<double>(&result4) != nullptr);
    double root2 = std::get<double>(result4);
    assert(std::abs(root2 - 2.0) < epsilon);
    std::cout << "Test 3 (linear equation): passed\n";

    auto result5 = solve(0.0, 0.0, 0.0);
    assert(std::get_if<std::monostate>(&result5) != nullptr);
    std::cout << "Test 4 (0=0): passed\n";

    auto result6 = solve(0.0, 0.0, 5.0);
    assert(std::get_if<std::monostate>(&result6) != nullptr);
    std::cout << "Test 5 (0=5): passed\n";

    auto result7 = solve(1.0, 0.0, 0.0);
    assert(std::get_if<double>(&result7) != nullptr);
    double root3 = std::get<double>(result7);
    assert(std::abs(root3) < epsilon);
    std::cout << "Test 6 (zero root): passed\n";

    std::cout << "\nAll tests passed!\n";

}
