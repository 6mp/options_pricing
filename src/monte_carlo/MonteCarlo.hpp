#pragma once

#include <concepts>
#include <random>
#include <type_traits>

template <typename Ty>
concept numeric = std::is_arithmetic_v<Ty>;

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

  auto runSimulation(numeric auto lower_bound, numeric auto upper_bound,
                     std::size_t iterations,
                     std::invocable<std::size_t, double> auto func) -> void {

    using lower_bound_ty = decltype(lower_bound);
    using upper_bound_ty = decltype(upper_bound);

    static_assert(std::is_same_v<lower_bound_ty, upper_bound_ty>,
                  "lower_bound and upper_bound must have the same type");

    if constexpr (std::is_floating_point_v<lower_bound_ty>) {

      std::uniform_real_distribution<lower_bound_ty> dist{lower_bound,
                                                          upper_bound};

      for (std::size_t i{}; i < iterations; i++) {
        func(i, dist(rand_engine));
      }
    } else if constexpr (std::is_integral_v<lower_bound_ty>) {

      std::uniform_int_distribution<lower_bound_ty> dist{lower_bound,
                                                         upper_bound};

      for (std::size_t i{}; i < iterations; i++) {
        func(i, dist(rand_engine));
      }
    }
  }
};
