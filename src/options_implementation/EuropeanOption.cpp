#include <options_implementation/EuropeanOption.hpp>
#include <monte_carlo/MonteCarlo.hpp>

#include <numeric>


auto EuropeanOption::calculatePayoff(EOptionType option_type, double spot) const -> double {
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
        throw std::runtime_error("Unsupported option type specified.");
    }
}


auto EuropeanOption::calculateBSPrice(EOptionType option_type) const -> double
{
    MonteCarlo simulator{};

    const double s_adjust = m_spot * std::exp(m_timeToExpiry * (m_riskFreeRate - std::pow(m_volatility, 2) * .5));

    const std::vector<double> prices = simulator.runSimulation(
        m_numberOfPaths,
        [s_adjust, this](const std::array<double, 1>& var)
        { return s_adjust * std::exp(std::sqrt(std::pow(m_volatility, 2) * m_timeToExpiry) * var[0]); });

    const double payoff_sum = std::accumulate(
        prices.begin(),
        prices.end(),
        0.0,
        [this, option_type](const auto& accum, const auto& cur) { return accum + calculatePayoff(option_type, cur); });

    return (payoff_sum / static_cast<double>(m_numberOfPaths)) * std::exp(-m_riskFreeRate * m_timeToExpiry);
}
