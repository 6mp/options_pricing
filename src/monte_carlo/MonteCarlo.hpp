#pragma once

#include <array>
#include <concepts>
#include <functional>
#include <random>
#include <type_traits>

namespace
{
// clang-format off
    template<typename Ty>
    concept numeric = std::is_arithmetic_v<Ty>;

    template<typename FunTy, typename RetTy, std::size_t VarCount>
    concept callback = std::invocable<FunTy, const std::array<double, VarCount>&> 
                       && std::is_same_v<std::invoke_result_t<FunTy, const std::array<double, VarCount>&>, RetTy>;
// clang-format on
}    // namespace

class MonteCarlo
{
private:
    std::mt19937_64 m_randEngine;

public:
    MonteCarlo()
    {
        std::random_device r;
        std::seed_seq seed{r(), r(), r()};
        m_randEngine = std::mt19937_64{seed};
    }

    // copy ctor and assignment deleted
    MonteCarlo(const MonteCarlo& other) = delete;
    auto operator=(const MonteCarlo& rhs) -> MonteCarlo& = delete;

    // defaulted move copy and assignment
    MonteCarlo(MonteCarlo&& other) = default;
    auto operator=(MonteCarlo&& rhs) -> MonteCarlo& = default;

    /**
     * \brief 
     * \tparam VarCount Return type of callback function
     * \tparam RetTy Number of random variables which will be passed to callback function through a const std::array
     * \param iterations Number of iterations to run
     * \param func Callback function which will be supplied with random variables and returns some value which is the collected in a std::vector
     * \return collection of callbacks results
     */
    template<std::size_t VarCount = 1, typename RetTy = double>
    [[nodiscard("do not discard this result")]] auto runSimulation(
       std::size_t iterations, callback<RetTy, VarCount> auto func)
        -> std::vector<RetTy>;
};

template<std::size_t VarCount, typename RetTy>
auto MonteCarlo::runSimulation(
    std::size_t iterations,
    callback<RetTy, VarCount> auto func) -> std::vector<RetTy> {

    // pre allocate vector
    std::vector<RetTy> output{};
    output.reserve(iterations);

    std::normal_distribution dist{};


    std::array<double, VarCount> in{};
    for (std::size_t idx{}; idx < iterations; idx++)
    {
        for (std::size_t cur_var{}; cur_var < VarCount; cur_var++)
        {
            in[cur_var] = dist(m_randEngine);
        }
        output.emplace_back(func(in));
    }

    // rvo
    return output;
}
