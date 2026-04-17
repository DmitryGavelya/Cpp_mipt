#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <limits>
#include <boost/graph/adjacency_matrix.hpp>

int main() {
  const int num_vertices = 10;
  typedef boost::adjacency_matrix<boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
  Graph g(num_vertices);

  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);

  for (int i = 0; i < num_vertices; ++i) {
    for (int j = i + 1; j < num_vertices; ++j) {
      boost::add_edge(i, j, dist(gen), g);
    }
  }

  auto weight_map = boost::get(boost::edge_weight, g);

  for (int i = 0; i < num_vertices; ++i) {
    for (int j = 0; j < num_vertices; ++j) {
      if (i == j) {
        std::cout << "0\t";
      } else {
        auto e = boost::edge(i, j, g);
        std::cout << weight_map[e.first] << "\t";
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  std::vector<int> vertices(num_vertices);
  std::iota(vertices.begin(), vertices.end(), 0);

  int min_cost = std::numeric_limits<int>::max();
  std::vector<int> best_path;

  do {
    int current_cost = 0;
    for (int i = 0; i < num_vertices - 1; ++i) {
      auto e = boost::edge(vertices[i], vertices[i + 1], g);
      current_cost += weight_map[e.first];
    }
    auto e_last = boost::edge(vertices.back(), vertices.front(), g);
    current_cost += weight_map[e_last.first];

    if (current_cost < min_cost) {
      min_cost = current_cost;
      best_path = vertices;
    }
  } while (std::next_permutation(vertices.begin() + 1, vertices.end()));

  for (int v : best_path) {
    std::cout << v << " ";
  }
  std::cout << best_path.front() << "\n";
  std::cout << min_cost << "\n";
}
