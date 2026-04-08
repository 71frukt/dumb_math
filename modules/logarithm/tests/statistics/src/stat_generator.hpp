#pragma once

#include <ostream>
#include <cmath>
#include <limits>
#include <concepts>
#include <cstdint>
#include <iomanip>

#include "calc_ln_ulp_err.hpp"

namespace dumb_math        {
namespace logarithm::tests {
namespace statistics       {

template <std::floating_point T>
void GenerateLnErrorUlpData_1toInf(std::ostream& ostream) 
{
    constexpr int min_exp = std::numeric_limits<T>::min_exponent;
    constexpr int max_exp = std::numeric_limits<T>::max_exponent;

    constexpr uint64_t SAMPLES_PER_EXP = 256; 
    const T step_val = T(1.0) / static_cast<T>(SAMPLES_PER_EXP);

    for (int exp = min_exp; exp < max_exp; ++exp) 
    {
        for (uint64_t i = 0; i < SAMPLES_PER_EXP; ++i) 
        {
            // mantissa = [1.0, 2.0)
            T mantissa = T(1.0) + static_cast<T>(i) * step_val;

            // x = mantissa * 2^exp
            T x = std::scalbn(mantissa, exp);

            T ulp = CalcLnUlpErr(x);

            ostream << std::left << std::setw(30) 
                    << std::defaultfloat << std::setprecision(std::numeric_limits<T>::max_digits10) << x 
                    << std::fixed        << std::setprecision(std::numeric_limits<T>::max_digits10) << ulp 
                    << "\n";
        }
    }
}

template <std::floating_point T>
void GenerateLnErrorUlpData_near1(std::ostream& ostream)
{ 
    // окрестность 1.0 - самая критическая зона
    
    T x_right = T(1.0);
    T x_left  = T(1.0);

    // 1000 ближайших чисел справа от 1.0
    for (int i = 0; i < 1000; ++i) 
    {
        x_right = std::nextafter(x_right, std::numeric_limits<T>::infinity());

        if (x_right != T(1.0))
        {
            ostream << std::left << std::setw(30) 
                    << std::defaultfloat << std::setprecision(std::numeric_limits<T>::max_digits10) << x_right 
                    << std::fixed        << std::setprecision(std::numeric_limits<T>::max_digits10) << CalcLnUlpErr(x_right) 
                    << "\n";
        }
    }

    // 1000 ближайших чисел слева от 1.0
    for (int i = 0; i < 1000; ++i) 
    {
        x_left = std::nextafter(x_left, T(0.0));

        if (x_left != T(1.0))
        {
            ostream << std::left << std::setw(30) 
                    << std::defaultfloat << std::setprecision(std::numeric_limits<T>::max_digits10) << x_left 
                    << std::fixed        << std::setprecision(std::numeric_limits<T>::max_digits10) << CalcLnUlpErr(x_left) 
                    << "\n";
        }
    }
}

} // namespace statistics
} // namespace logarithm::tests
} // namespace dumb_math