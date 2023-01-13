#include <iostream>
#include <format>

#include "options_implementation/EuropeanOption.hpp"


auto main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) -> int
{
    const EuropeanOption::Inputs input{
        .strike = 100.0,
        .spot = 100.0,
        .volatility = .2,
        .time_to_expiry = 1.0,
        .risk_free_rate = .05,
        .number_of_paths = 100000};


    const EuropeanOption euro_opt{input};
    const auto call_price = euro_opt.calculateBSPrice(EuropeanOption::EOptionType::CALL);
    std::cout << call_price << "\n";

    const auto put_price = euro_opt.calculateBSPrice(EuropeanOption::EOptionType::PUT);
    std::cout << put_price << "\n";

    return 0;
}