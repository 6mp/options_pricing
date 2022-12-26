#include "MonteCarlo.hpp"
#include <random>

/* auto MonteCarlo::runSimulation(numeric auto lower_bound,
                               numeric auto upper_bound, std::size_t iterations,
                               std::invocable<std::size_t, double> auto func) -> int {

  static_assert(std::is_same_v<decltype(lower_bound), decltype(upper_bound)>,
                "lower_bound and upper_bound must have the same type");

  std::uniform_real_distribution<decltype(lower_bound)> dist{};

  for (std::size_t i{}; i < iterations; i++) {
    func(i, rand_engine(dist)); 
  }
} */
