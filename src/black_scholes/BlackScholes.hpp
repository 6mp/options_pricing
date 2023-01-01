#pragma once

enum EBSType {
    EUROCALL,
    EUROPUT
};

class BlackScholes {
    double m_strike{};
    double m_spot{};
    double m_volatility{};
    double m_timeToExpiry{};
    double m_riskFreeRate{};

    BlackScholes(double strike, double spot, double volatility, double time_to_expriy, double risk_free_rate) :
        m_strike(strike),
        m_spot(spot),
        m_volatility(volatility),
        m_timeToExpiry(time_to_expriy),
        m_riskFreeRate(risk_free_rate) {}

    template<EBSType OptionTy>
    auto calculatePrice(std::size_t runs) -> double;
};

// time to payoff is time in days / 365
