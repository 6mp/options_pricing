#include "monte_carlo/MonteCarlo.hpp"
#include <iostream>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {

  MonteCarlo simulator{};

  simulator.runSimulation(
      0, 100, 100, [](std::size_t iteration, double random_variable) {
        std::cout << "iter:" << iteration << " with var:" << random_variable
                  << "\n";
      });

  

  return 0;
}
