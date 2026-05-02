///////////////////////////////////////////////////////////////////////////////////////////////////

#include <print>
#include <cmath>
#include <string>
#include <string_view>
#include <vector>
#include <variant>

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////

class Operand : public boost::spirit::x3::variant<

    double,
    boost::spirit::x3::forward_ast<struct Sign>,
    boost::spirit::x3::forward_ast<struct Factorial>,
    boost::spirit::x3::forward_ast<struct List>

>
{
public :
    using base_type::base_type, base_type::operator=;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

struct Sign     { char    operation = '\0'; Operand operand; };
struct Factorial{                           Operand operand; };
struct Step     { char    operation = '\0'; Operand operand; };

///////////////////////////////////////////////////////////////////////////////////////////////////

struct List
{
    Operand              head;
    std::vector<Step>    steps;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOST_FUSION_ADAPT_STRUCT(Sign,      operation, operand)
BOOST_FUSION_ADAPT_STRUCT(Factorial,            operand)
BOOST_FUSION_ADAPT_STRUCT(Step,      operation, operand)
BOOST_FUSION_ADAPT_STRUCT(List,      head, steps)

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace parser
{
    namespace x3 = boost::spirit::x3;

    x3::rule<struct rule_expr_tag,    List>    rule_expr;
    x3::rule<struct rule_term_tag,    List>    rule_term;
    x3::rule<struct rule_power_tag,   List>    rule_power;
    x3::rule<struct rule_postfix_tag, Operand> rule_postfix;
    x3::rule<struct rule_unary_tag,   Operand> rule_unary;
    x3::rule<struct rule_primary_tag, Operand> rule_primary;

    auto rule_expr_def = rule_term >> *(
        (x3::char_('+') >> rule_term) |
        (x3::char_('-') >> rule_term)
    );

    auto rule_term_def = rule_power >> *(
        (x3::char_('*') >> rule_power) |
        (x3::char_('/') >> rule_power) |
        (x3::char_('%') >> rule_power)
    );

    auto rule_power_def = rule_postfix >> *(
        x3::char_('^') >> rule_postfix
    );

    auto rule_postfix_def =
        (rule_unary >> x3::char_('!')) |
        rule_unary;

    auto rule_unary_def =
        (x3::char_('+') >> rule_unary) |
        (x3::char_('-') >> rule_unary) |
        rule_primary;

    auto rule_primary_def =
        x3::double_                          |
        ('(' >> rule_expr >> ')')            |
        ('[' >> rule_expr >> ']')            |
        ('{' >> rule_expr >> '}');

    BOOST_SPIRIT_DEFINE(rule_expr, rule_term, rule_power, rule_postfix, rule_unary, rule_primary)

}

///////////////////////////////////////////////////////////////////////////////////////////////////

class Calculator
{
public :

    auto operator()(double x) const -> double
    {
        return x;
    }

    auto operator()(Sign const & sign) const -> double
    {
        auto x = boost::apply_visitor(*this, sign.operand);
        return (sign.operation == '-') ? -x : x;
    }

    auto operator()(Factorial const & f) const -> double
    {
        auto x = boost::apply_visitor(*this, f.operand);
        auto n = static_cast<long long>(x);
        double result = 1.0;
        for (long long i = 2; i <= n; ++i) result *= i;
        return result;
    }

    auto operator()(Step const & step, double x) const -> double
    {
        auto y = boost::apply_visitor(*this, step.operand);
        switch (step.operation)
        {
        case '+' : return x + y;
        case '-' : return x - y;
        case '*' : return x * y;
        case '/' : return x / y;
        case '%' : return std::fmod(x, y);
        case '^' : return std::pow(x, y);
        }
        return x;
    }

    auto operator()(List const & list) const -> double
    {
        auto x = boost::apply_visitor(*this, list.head);
        for (auto const & step : list.steps)
            x = (*this)(step, x);
        return x;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

auto parse(std::string_view view)
{
    auto begin = std::begin(view), end = std::end(view);

    using parser::rule_expr;
    using boost::spirit::x3::ascii::space;

    List list;
    boost::spirit::x3::phrase_parse(begin, end, rule_expr, space, list);

    static Calculator calculator;
    return calculator(list);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    std::string string;

    std::print("main : enter std::string string(s) : \n");

    while (std::getline(std::cin >> std::ws, string))
    {
        if (string.front() != ';')
            std::print("main : {} = {}\n", string, parse(string));
        else
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////