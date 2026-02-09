#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>

int main() {
    const std::string target = "methinksitislikeaweasel";
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    const std::size_t population_size = 100;
    const double mutation_prob = 0.05;

    std::random_device rd;
    std::default_random_engine eng{rd()};
    std::uniform_int_distribution<int> letter_dist(0, static_cast<int>(alphabet.size() - 1));
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    auto random_string = [&](std::size_t n) {
        std::string s;
        s.reserve(n);
        for (std::size_t i = 0; i < n; ++i)
            s.push_back(alphabet[letter_dist(eng)]);
        return s;
    };

    auto mutate = [&](const std::string& parent) {
        std::string child = parent;
        for (char& ch : child) {
            if (prob_dist(eng) < mutation_prob)
                ch = alphabet[letter_dist(eng)];
        }
        return child;
    };

    auto metric = [&](const std::string& s) {
        int dist = 0;
        for (std::size_t i = 0; i < s.size(); ++i)
            if (s[i] != target[i]) ++dist;
        return dist;
    };

    std::string current = random_string(target.size());

    std::size_t generation = 0;
    while (true) {
        std::cout << "gen " << generation << ": " << current << '\n';

        if (current == target)
            break;

        std::vector<std::string> population;
        population.reserve(population_size);
        for (std::size_t i = 0; i < population_size; ++i)
            population.push_back(mutate(current));

        int best_metric = std::numeric_limits<int>::max();
        std::size_t best_idx = 0;
        for (std::size_t i = 0; i < population_size; ++i) {
            int m = metric(population[i]);
            if (m < best_metric) {
                best_metric = m;
                best_idx = i;
            }
        }

        current = population[best_idx];
        if (best_metric == 0) {
            ++generation;
            std::cout << "gen " << generation << ": " << current << '\n';
            break;
        }

        ++generation;
    }
}
