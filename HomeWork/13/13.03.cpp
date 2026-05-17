#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <filesystem>
#include <algorithm>

void transform(std::string const & path_1, std::string const & path_2)
{
    auto string = (std::stringstream() << std::fstream(path_1, std::ios::in).rdbuf()).str();

    for (auto iterator = std::begin(string); iterator != std::end(string); ++iterator)
    {
        if (*iterator == '\'')
        {
            do { ++iterator; }
            while (!(*iterator == '\'' && *std::prev(iterator) != '\\'));
        }

        if (*iterator == '\"')
        {
            do { ++iterator; }
            while (!(*iterator == '\"' && *std::prev(iterator) != '\\'));
        }

        if (*iterator == 'R' && std::next(iterator) != std::end(string) && *std::next(iterator) == '"')
        {
            ++iterator;
            ++iterator;

            auto delim_begin = iterator;

            while (*iterator != '(') ++iterator;

            auto delim = std::string(delim_begin, iterator);

            auto close = delim + ")\"";

            auto pos = string.find(close, std::distance(std::begin(string), iterator));

            if (pos != std::string::npos)
            {
                iterator = std::begin(string) + pos + close.size() - 1;
            }

            continue;
        }

        if (*iterator == '/')
        {
            if (std::next(iterator) != std::end(string) && *std::next(iterator) == '/')
            {
                auto end = std::next(iterator, 2);
                while (end != std::end(string) && *end != '\n') ++end;
                iterator = string.erase(iterator, end);
            }
            else if (std::next(iterator) != std::end(string) && *std::next(iterator) == '*')
            {
                auto end = std::next(iterator, 3);
                while (!(*end == '/' && *std::prev(end) == '*')) ++end;
                iterator = string.erase(iterator, ++end);
            }
        }

        if (iterator == std::end(string)) break;
    }

    auto is_blank_line = [](std::string const & line) -> bool
    {
        return std::all_of(std::begin(line), std::end(line),
            [](char c) { return c == ' ' || c == '\t' || c == '\r'; });
    };

    std::stringstream ss(string);
    std::stringstream result;
    std::string line;

    while (std::getline(ss, line))
    {
        if (!is_blank_line(line))
        {
            result << line << '\n';
        }
    }

    std::fstream(path_2, std::ios::out) << result.str();
}

int main()
{
    auto path_1 = "source.cpp";
    auto path_2 = "output.cpp";

    transform(path_1, path_2);

    std::print("main : enter char : "); std::cin.get();

    std::filesystem::remove(path_2);
}