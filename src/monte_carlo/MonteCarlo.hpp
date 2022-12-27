#pragma once

#include <concepts>
#include <random>
#include <type_traits>

template <typename Ty>
concept numeric = std::is_arithmetic_v<Ty>;

// requires that function is callable with current iteration and randomly
// generated variable and then returns RetTy which by default is a double
template <typename Fn, typename RetTy, typename BoundTy>
concept callback_with_iteration = requires(Fn f) {
                                    {
                                      f(std::size_t{}, BoundTy{})
                                      } -> std::same_as<RetTy>;
                                  };

template <typename Fn, typename RetTy, typename BoundTy>
concept callback_one_arg = requires(Fn f) {
                             { f(BoundTy{}) } -> std::same_as<RetTy>;
                           };

class MonteCarlo {
private:
  std::mt19937_64 rand_engine{};

public:
  MonteCarlo() = default;

  // copy ctor and assignment deleted
  MonteCarlo(const MonteCarlo &other) = delete;
  auto operator=(const MonteCarlo &rhs) -> MonteCarlo & = delete;

  // defaulted move copy and assignment
  MonteCarlo(MonteCarlo &&other) = default;
  auto operator=(MonteCarlo &&rhs) -> MonteCarlo & = default;

  // this could techincally be used with just doubles as the random variable
  template <typename RetTy = double, numeric BoundTy>
  [[nodiscard("dont discard this result bruv")]] auto
  runSimulation(BoundTy lower_bound, BoundTy upper_bound,
                std::size_t iterations,
                callback_one_arg<RetTy, BoundTy> auto func)
      -> std::vector<RetTy> {

    // pre allocate vector
    std::vector<RetTy> output{};
    output.reserve(iterations);

    if constexpr (std::is_floating_point_v<BoundTy>) {

      std::uniform_real_distribution<BoundTy> dist{lower_bound, upper_bound};

      for (std::size_t i{}; i < iterations; i++) {
        output.emplace_back(func(dist(rand_engine)));
      }
    } else if constexpr (std::is_integral_v<BoundTy>) {

      std::uniform_int_distribution<BoundTy> dist{lower_bound, upper_bound};

      for (std::size_t i{}; i < iterations; i++) {
        output.emplace_back(func(dist(rand_engine)));
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
