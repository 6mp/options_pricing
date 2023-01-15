#pragma once
#include <cstddef> 

#include <monte_carlo/MonteCarlo.hpp>

class EuropeanOption
{
    double m_strike{};
    double m_spot{};
    double m_volatility{};    // sigma
    double m_timeToExpiry{};
    double m_riskFreeRate{};
    std::size_t m_numberOfPaths{};

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

    enum class EOptionType
    {
        CALL,
        PUT
    };

    enum class EPayoffType
    {
        VANILLA,
        DIGITAL
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

    explicit(false) EuropeanOption(const Inputs& inputs)
        : m_strike(inputs.strike)
        , m_spot(inputs.spot)
        , m_volatility(inputs.volatility)
        , m_timeToExpiry(inputs.time_to_expiry)
        , m_riskFreeRate(inputs.risk_free_rate)
        , m_numberOfPaths(inputs.number_of_paths)
    {}

    /**
     * \brief Calculates price of option using black-scholes formula
     * \param option_type Call or Put
     * \return Price of option
     */
    template<EPayoffType Ty>
    [[nodiscard]] auto calculateBSPrice(EOptionType option_type) const -> double;

private:
    /**
     * \brief Calculate vanilla payoff of option given current spot price
     * \param option_type Call or Put
     * \param spot Current spot price
     * \return payoff of option
     */
    [[nodiscard]] auto calculateVanillaPayoff(EOptionType option_type, double spot) const -> double;


    /**
     * \brief Calculate digital payoff of option given current spot price
     * \param option_type Call or Put
     * \param spot Current spot price
     * \return payoff of option
     */
    [[nodiscard]] auto calculateDigitalPayoff(EOptionType option_type, double spot) const -> double;
};

template<EuropeanOption::EPayoffType PayoffTy>
auto EuropeanOption::calculateBSPrice(EOptionType option_type) const -> double
{
    MonteCarlo simulator{};

    // adjusted spot is the spot price of the underlying asset adjusted for the risk-free rate.
    const double adjusted_spot = m_spot * std::exp(m_timeToExpiry * (m_riskFreeRate - std::pow(m_volatility, 2) * .5));

    const double payoff_sum = simulator.runSimulationSum(
        m_numberOfPaths,
        [adjusted_spot, this, option_type](const std::array<double, 1>& var)
        {
            // calculate the new spot price of the underlying asset
            const auto new_spot =
                adjusted_spot * std::exp(std::sqrt(std::pow(m_volatility, 2) * m_timeToExpiry) * var[0]);

            if constexpr (PayoffTy == EPayoffType::VANILLA)
            {
                return calculateVanillaPayoff(option_type, new_spot);
            }
            else if constexpr (PayoffTy == EPayoffType::DIGITAL)
            {
                return calculateDigitalPayoff(option_type, new_spot);
            }
        });

    // average all payoffs and discount the result
    return (payoff_sum / static_cast<double>(m_numberOfPaths)) * std::exp(-m_riskFreeRate * m_timeToExpiry);
}
