#include <options_implementation/EuropeanOption.hpp>

#include <numeric>

auto EuropeanOption::calculateVanillaPayoff(EOptionType option_type, double spot)  const -> double 
{
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

auto EuropeanOption::calculateDigitalPayoff(EOptionType option_type, double spot) const -> double
{
    // conversion from bool to double
    // price > 0 => true => 1.0
    return calculateVanillaPayoff(option_type, spot) > 0;
}

