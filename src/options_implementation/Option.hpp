#pragma once
#include <cstddef> 
#include <numeric> // numeric
#include <execution>


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
    std::size_t m_numberOfPaths{};

    auto calculatePayoff(EOptionType option_type, double spot) const -> double
    {
        if (option_type == EOptionType::EUROCALL)
        {
            return std::max(spot - m_strike, 0.0);
        }
        else if (option_type == EOptionType::EUROPUT)
        {
            return std::max(m_strike - spot, 0.0);
        }
        else
        {
            return 0;
        }
    }

public:
    Option(
        double strike,
        double spot,
        double volatility,
        double time_to_expiry,
        double risk_free_rate,
        std::size_t number_of_paths)
        : m_strike(strike)
        , m_spot(spot)
        , m_volatility(volatility)
        , m_timeToExpiry(time_to_expiry)
        , m_riskFreeRate(risk_free_rate)
        , m_numberOfPaths(number_of_paths)
    {}

    auto calculatePrice(EOptionType option_type) const -> double
    {
        MonteCarlo simulator{};
        //static double last_price{m_spot};
        
        static double S_cur = 0.0;    // Our current asset price ("spot")
        static double S_adjust =
            m_spot *
            std::exp(
                m_timeToExpiry *
                (m_riskFreeRate - 0.5 * std::pow(m_volatility, 2)));    // The adjustment to the spot price

        auto get_price = [this](const std::array<double, 1>& var) -> double
        {
            S_cur = S_adjust * std::exp(
                                   sqrt(std::pow(m_volatility, 2) * m_timeToExpiry) *
                                   var[0]);    // Adjust the spot price via the Brownian motion final distribution

            return S_cur;
        };

        std::vector<double> terminal_prices = simulator.runSimulation(0.0, 1.0, m_numberOfPaths, get_price);

        std::vector<double> payoffs{};
        payoffs.resize(m_numberOfPaths);

        std::ranges::transform(
            terminal_prices,
            payoffs.begin(),
            [option_type, this](const auto& price) { return calculatePayoff(option_type, price); });


        // Average the pay-off sum via the number of paths and then
        // discount the risk-free rate from the price
        return (std::reduce(payoffs.begin(), payoffs.end()) / static_cast<double>(m_numberOfPaths)) * std::exp(-m_riskFreeRate * m_timeToExpiry);
    }
};

// time to payoff is time in days / 365
