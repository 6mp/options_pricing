#include "monte_carlo/MonteCarlo.hpp"
#include <cstdint>
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {

  MonteCarlo simulator{};

  // integration example of f(x) = cos(2 * pi * x^2) on the interval 0 to 1

  constexpr auto lower_bound{0.0}, upper_bound{1.0};
  constexpr auto iterations{10000};

  auto function = [](const double &x) {
    return std::cos(2 * std::numbers::pi * std::pow(x, 2));
  };

  std::vector<double> runs =
      simulator.runSimulation(lower_bound, upper_bound, iterations, function);

  double count = static_cast<double>(runs.size());
  double avg = std::reduce(runs.begin(), runs.end()) / count;
  double expected_value = (upper_bound - lower_bound) * avg;

  std::cout << "expected integral value after " << iterations
            << " iterations is " << expected_value << "\n";

  return 0;
}
