#pragma once

#include <array>
#include <concepts>
#include <functional>
#include <random>
#include <type_traits>

template<typename Ty>
concept numeric = std::is_arithmetic_v<Ty>;

class MonteCarlo {
private:
    std::mt19937_64 m_randEngine;

public:
    MonteCarlo() {
        std::random_device r;
        std::seed_seq seed{r(), r()};
        m_randEngine = std::mt19937_64{seed};
    }

    // copy ctor and assignment deleted
    MonteCarlo(const MonteCarlo& other) = delete;
    auto operator=(const MonteCarlo& rhs) -> MonteCarlo& = delete;

    // defaulted move copy and assignment
    MonteCarlo(MonteCarlo&& other) = default;
    auto operator=(MonteCarlo&& rhs) -> MonteCarlo& = default;

    /// \tparam RetTy Return type of callback function
    /// \tparam BoundTy Type of randomly generated variable
    /// \tparam VarCount Number of random variables which will be passed to callback function through a const std::array reference
    /// \param lower_bound Lower bound for generated variables
    /// \param upper_bound Upper bound for generated variables
    /// \param iterations Number of iterations to run
    /// \param func Callback function which will be supplied with random variables and returns some value which is the collected in a std::vector
    /// \return collection of callbacks results
    template<typename RetTy = double, numeric BoundTy, std::size_t VarCount>
    [[nodiscard]] auto
    runSimulation(BoundTy lower_bound, BoundTy upper_bound,
                  std::size_t iterations,
                  RetTy (*func)(const std::array<BoundTy, VarCount>&))
            -> std::vector<RetTy> {

        // pre allocate vector
        std::vector<RetTy> output{};
        output.reserve(iterations);

        if constexpr (std::is_floating_point_v<BoundTy>) {
            std::uniform_real_distribution<BoundTy> dist{lower_bound, upper_bound};

            std::array<BoundTy, VarCount> in{};
            for (std::size_t cur_iter{}; cur_iter < iterations; cur_iter++) {
                for (int cur_var = 0; cur_var < VarCount; cur_var++) {
                    in[cur_var] = dist(m_randEngine);
                }
                output.emplace_back(func(in));
            }
        } else if constexpr (std::is_integral_v<BoundTy>) {
            std::uniform_int_distribution<BoundTy> dist{lower_bound, upper_bound};

            std::array<BoundTy, VarCount> in{};
            for (std::size_t cur_iter{}; cur_iter < iterations; cur_iter++) {
                for (int cur_var = 0; cur_var < VarCount; cur_var++) {
                    in[cur_var] = dist(m_randEngine);
                }
                output.emplace_back(func(in));
            }
        } else {
            static_assert(
                    std::is_floating_point_v<BoundTy> || std::is_integral_v<BoundTy>,
                    "supplied bound type was numeric but not integral or floating point");
        }

        // rvo
        return output;
    }
};
