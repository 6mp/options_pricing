#pragma once
#include <cstddef>

#include "../monte_carlo/MonteCarlo.hpp"
enum class EOptionType
{
    EUROCALL,
    EUROPUT
};

class Option
{
    double m_strike{};
    double m_spot{};
    double m_volatility{};
    double m_timeToExpiry{};
    double m_riskFreeRate{};

    template<EOptionType OptionTy>
    auto calculatePayoff() -> double
    {
        if constexpr (OptionTy == EOptionType::EUROCALL)
        {
            return std::max(m_spot - m_strike, 0.0);
        }
        else if constexpr (OptionTy == EOptionType::EUROPUT)
        {
            return std::max(m_strike - m_spot, 0.0);
        }
    }

public:
    Option(double strike, double spot, double volatility, double time_to_expiry, double risk_free_rate)
        : m_strike(strike)
        , m_spot(spot)
        , m_volatility(volatility)
        , m_timeToExpiry(time_to_expiry)
        , m_riskFreeRate(risk_free_rate)
    {}


    template<EOptionType OptionTy>
    auto calculatePrice(int number_of_paths) -> double
    {
        MonteCarlo simulator{};
        static double last_price{};

        auto get_price = [](const std::array<double, 1>& var) -> double {
              
        };

        std::vector<double> results = simulator.runSimulation(0.0, 1.0, number_of_paths, get_price);
        return 0;
    }
};

// time to payoff is time in days / 365
