#include <options_implementation/EuropeanOption.hpp>
#include <monte_carlo/MonteCarlo.hpp>

#include <numeric>


auto EuropeanOption::calculatePayoff(EOptionType option_type, double spot) const -> double {
    if (option_type == EOptionType::CALL)
    {
        return std::max(spot - m_strike, 0.0);
    }
    else if (option_type == EOptionType::PUT)
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

    const double adjusted_spot = m_spot * std::exp(m_timeToExpiry * (m_riskFreeRate - std::pow(m_volatility, 2) * .5));

    const double payoff_sum = simulator.runSimulationSum(
        m_numberOfPaths,
        [adjusted_spot, this, option_type](const std::array<double, 1>& var)
        {
            const auto new_spot =
                adjusted_spot * std::exp(std::sqrt(std::pow(m_volatility, 2) * m_timeToExpiry) * var[0]);
            return calculatePayoff(option_type, new_spot);
        });

    return (payoff_sum / static_cast<double>(m_numberOfPaths)) * std::exp(-m_riskFreeRate * m_timeToExpiry);
}
