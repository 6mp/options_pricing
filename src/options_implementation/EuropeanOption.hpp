#pragma once
#include <cstddef> 

#include "../monte_carlo/MonteCarlo.hpp"

enum class EOptionType
{
    EUROCALL,
    EUROPUT
};

class EuropeanOption
{
    double m_strike{};
    double m_spot{};
    double m_volatility{};
    double m_timeToExpiry{};
    double m_riskFreeRate{};
    std::size_t m_numberOfPaths{};

    /**
     * \brief Calculate payoff of option given current spot price
     * \param option_type Call or Put
     * \param spot Current spot price
     * \return payoff of option
     */
    [[nodiscard]] auto calculatePayoff(EOptionType option_type, double spot) const -> double;

public:
    struct Inputs
    {
        double strike{};
        double spot{};
        double volatility{};
        double time_to_expiry{};
        double risk_free_rate{};
        std::size_t number_of_paths{};
    };

    EuropeanOption(
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

    EuropeanOption(const Inputs& inputs)
        : m_strike(inputs.strike)
        , m_spot(inputs.spot)
        , m_volatility(inputs.volatility)
        , m_timeToExpiry(inputs.time_to_expiry)
        , m_riskFreeRate(inputs.risk_free_rate)
        , m_numberOfPaths(inputs.number_of_paths)
    {}

    /**
     * \brief Calculates price of option using black scholes formula
     * \param option_type Call or Put
     * \return Price of option
     */
    [[nodiscard]] auto calculateBSPrice(EOptionType option_type) const -> double;
};

// time to payoff is time in days / 365
