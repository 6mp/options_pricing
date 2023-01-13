#pragma once

#include <array>
#include <concepts>
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

    template<typename Ty>
    concept addable = requires (Ty a, Ty b) {
        a + b;
        a += b;
    };
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

    ~MonteCarlo() = default;

    // copy ctor and assignment deleted
    MonteCarlo(const MonteCarlo& other) = delete;
    auto operator=(const MonteCarlo& rhs) -> MonteCarlo& = delete;

    // defaulted move copy and assignment
    MonteCarlo(MonteCarlo&& other) = default;
    auto operator=(MonteCarlo&& rhs) -> MonteCarlo& = default;

    /**
     * \brief Function to run a monte carlo simulation and collect all the results into a vector
     * \tparam VarCount Number of random variables which will be passed to callback function through a const std::array
     * \tparam Distribution std::distribution type
     * \tparam RetTy Return type of callback function
     * \param iterations Number of iterations to run
     * \param func Callback function which will be supplied with random variables and returns some value which is the
     * collected in a std::vector
     * \return collection of callbacks results
     */
    template<std::size_t VarCount = 1, typename Distribution = std::normal_distribution<double>, addable RetTy = double>
    [[nodiscard]] auto runSimulation(std::size_t iterations, const callback<RetTy, VarCount> auto& func)
        -> std::vector<RetTy>;

    /**
     * \brief Function to run a monte carlo simulation and sum all the results, used when each result is not needed
     * \tparam VarCount Number of random variables which will be passed to callback function through a const std::array
     * \tparam Distribution std::distribution type
     * \tparam RetTy Return type of callback function
     * \param iterations Number of iterations to run
     * \param func Callback function which will be supplied with random variables and returns a value which is summed
     * \return sum of callback results
     */
    template<std::size_t VarCount = 1, typename Distribution = std::normal_distribution<double>, addable RetTy = double>
    [[nodiscard]] auto runSimulationSum(std::size_t iterations, const callback<RetTy, VarCount> auto& func) -> RetTy;
};


template<std::size_t VarCount, typename Distribution, addable RetTy>
auto MonteCarlo::runSimulation(
    std::size_t iterations,
    const callback<RetTy, VarCount> auto& func) -> std::vector<RetTy> {

    // pre allocate vector
    std::vector<RetTy> output{};
    output.reserve(iterations);

    Distribution dist{};

    std::array<double, VarCount> in{};
    for (std::size_t idx{}; idx < iterations; idx++)
    {
        // load random vars into array
        for (std::size_t cur_var{}; cur_var < VarCount; cur_var++)
        {
            in[cur_var] = dist(m_randEngine);
        }

        // call func and store result
        output.emplace_back(func(in));
    }

    // rvo
    return output;
}

template<std::size_t VarCount, typename Distribution, addable RetTy>
auto MonteCarlo::runSimulationSum(std::size_t iterations, const callback<RetTy, VarCount> auto& func) -> RetTy
{
    RetTy sum{};
    Distribution dist{};

    std::array<double, VarCount> in{};
    for (std::size_t idx{}; idx < iterations; idx++)
    {
        // load random vars into array
        for (std::size_t cur_var{}; cur_var < VarCount; cur_var++)
        {
            in[cur_var] = dist(m_randEngine);
        }

        // collect result of func
        sum += func(in);
    }

    return sum;
}
