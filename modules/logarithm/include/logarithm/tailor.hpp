#pragma once

#include <concepts>
#include <array>

#include "RLogSU/logger.hpp"

namespace dumb_math::logarithm {

namespace detail {

template <size_t M, typename T>
constexpr std::array<T, M> GenerateTailorCoeffs()
{
    std::array<T, M> coeffs{};

    for (size_t i = 0; i < M; ++i)
    {
        T denom = static_cast<T>(i + 1);
        coeffs[i] = ((i % 2 == 0) ? 1.0 : -1.0) / denom;
    }
    
    return coeffs;
}

} // namespace detail

template <size_t M, std::floating_point T>
T TailorLn(T x)
{
    RLSU_ASSERT((x >= 0 && x <= 2), "x={} is out of range [0; 2]", x);

    T row = 0;
    T r = x - 1;
    T r_pow = r;

    static constexpr std::array<T, M> coeffs = detail::GenerateTailorCoeffs<M, T>();

    for (size_t i = 0; i < M; ++i)
    {
        row += r_pow * coeffs[i]; 
        
        r_pow *= r;
    }

    return row;
}

} // namespace dumb_math