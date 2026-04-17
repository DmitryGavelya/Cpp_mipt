#include <iostream>
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

class Visitor {
public:
    void operator()(std::monostate) const {
        std::cout << "No real roots or infinite roots\n";
    }

    void operator()(double root) const {
        std::cout << "One root: " << root << "\n";
    }

    void operator()(std::pair<double, double> roots) const {
        std::cout << "Two roots: " << roots.first << ", " << roots.second << "\n";
    }
};

int main()
{
    Visitor visitor;

    auto result1 = solve(1.0, -2.0, 1.0);
    std::visit(visitor, result1);

    auto result2 = solve(1.0, 0.0, 1.0);
    std::visit(visitor, result2);

    auto result3 = solve(0.0, 2.0, -4.0);
    std::visit(visitor, result3);

    auto result4 = solve(0.0, 0.0, 0.0);
    std::visit(visitor, result4);

    auto result5 = solve(0.0, 0.0, 5.0);
    std::visit(visitor, result5);

    auto result6 = solve(1.0, 0.0, 0.0);
    std::visit(visitor, result6);

    auto result7 = solve(1.0, -3.0, 2.0);
    std::visit(visitor, result7);
}
