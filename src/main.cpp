#include "monte_carlo/MonteCarlo.hpp"
#include <cstdint>
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>

#include "black_scholes/BlackScholes.hpp"


/*
template<typename Ty>
auto integrate(Ty lower_bound, Ty upper_bound, auto&& function) {
    MonteCarlo simulator{};
    constexpr auto iterations{10000000};

    std::vector<double> runs =
            simulator.runSimulation(lower_bound, upper_bound, iterations,
                                    std::forward<decltype(function)>(function));

    double count = static_cast<double>(runs.size());
    double avg = std::reduce(runs.begin(), runs.end()) / count;
    double expected_val = (upper_bound - lower_bound) * avg;

    std::cout << "expected integral value after " << iterations
              << " iterations is " << expected_val << "\n";
}
*/

auto main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) -> int {

    /*    integrate(0.0, 1.0, [](std::array<double, 1> &x) {
        return std::cos(2 * std::numbers::pi * std::pow(x[0], 2));
    });*/

    MonteCarlo simulator{};
    constexpr auto iterations{100000};
    constexpr auto lower_bound{0.0}, upper_bound{1.0};


    std::vector<double> runs =
            simulator.runSimulation(
                    lower_bound, upper_bound, iterations,
                    +[](const std::array<double, 1>& x) {
                        return std::cos(2 * std::numbers::pi * std::pow(x[0], 2));
                    });

    double count = static_cast<double>(runs.size());
    double avg = std::reduce(runs.begin(), runs.end()) / count;
    double expected_val = (upper_bound - lower_bound) * avg;

    std::cout << "expected integral value after " << iterations
              << " iterations is " << expected_val << "\n";


    return 0;
}
