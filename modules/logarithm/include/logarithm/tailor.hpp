#pragma once

#include <concepts>
#include <array>

#include "RLogSU/logger.hpp"

namespace dumb_math::logarithm {

namespace detail {

template <size_t N, typename T>
constexpr std::array<T, N> GenerateTailorCoeffs()
{
    std::array<T, N> coeffs{};

    for (size_t i = 0; i < N; ++i)
    {
        T denom = static_cast<T>(i + 1);
        coeffs[i] = ((i % 2 == 0) ? 1.0 : -1.0) / denom;
    }
    
    return coeffs;
}

} // namespace detail

template <size_t N, std::floating_point T>
T TailorLn(T x)
{
    RLSU_ASSERT((x >= 0 && x <= 2), "x={} is out of range [0; 2]", x);

    T row = 0;
    T r = x - 1;
    T r_pow = r;

    static constexpr std::array<T, N> coeffs = detail::GenerateTailorCoeffs<N, T>();

    for (size_t i = 0; i < N; ++i)
    {
        row += r_pow * coeffs[i]; 
        
        r_pow *= r;
    }

    return row;
}

} // namespace dumb_math