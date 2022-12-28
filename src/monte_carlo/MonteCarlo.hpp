#pragma once

#include <array>
#include <concepts>
#include <functional>
#include <random>
#include <type_traits>

template<typename Ty>
concept numeric = std::is_arithmetic_v<Ty>;

template<typename Fn, typename RetTy, typename BoundTy, typename... Args>
concept callback = requires(Fn f, Args... args) {
                       { f(args...) } -> std::same_as<RetTy>;
                   };

class MonteCarlo {
private:
    std::mt19937_64 rand_engine{};

public:
    MonteCarlo() = default;

    // copy ctor and assignment deleted
    MonteCarlo(const MonteCarlo& other) = delete;
    auto operator=(const MonteCarlo& rhs) -> MonteCarlo& = delete;

    // defaulted move copy and assignment
    MonteCarlo(MonteCarlo&& other) = default;
    auto operator=(MonteCarlo&& rhs) -> MonteCarlo& = default;

    // this could technically be used with just doubles as the random variable
    template<typename RetTy = double, numeric BoundTy, std::size_t VarCount>
    [[nodiscard("dont discard this result bruv")]] auto
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
                    in[cur_var] = dist(rand_engine);
                }
                output.emplace_back(func(in));
            }
        } else if constexpr (std::is_integral_v<BoundTy>) {
            std::uniform_int_distribution<BoundTy> dist{lower_bound, upper_bound};

            std::array<BoundTy, VarCount> in{};
            for (std::size_t cur_iter{}; cur_iter < iterations; cur_iter++) {
                for (int cur_var = 0; cur_var < VarCount; cur_var++) {
                    in[cur_var] = dist(rand_engine);
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
