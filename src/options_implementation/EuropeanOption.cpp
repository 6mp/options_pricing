#include <options_implementation/EuropeanOption.hpp>

#include <numeric>

auto EuropeanOption::calculateVanillaPayoff(EOptionType option_type, double spot) const -> double
{
    switch (option_type)
    {
        case EOptionType::CALL: return std::max(spot - m_strike, 0.0);
        case EOptionType::PUT: return std::max(m_strike - spot, 0.0);
        default: throw EuropeanOptionException("Unsupported option type specified.");
    }
}

auto EuropeanOption::calculateDigitalPayoff(EOptionType option_type, double spot) const -> double
{
    // conversion from bool => true => 1.0
    switch (option_type)
    {
        case EOptionType::CALL: return spot - m_strike >= 0;
        case EOptionType::PUT: return m_strike - spot >= 0;
        default: throw EuropeanOptionException("Unsupported option type specified.");
    }
}
