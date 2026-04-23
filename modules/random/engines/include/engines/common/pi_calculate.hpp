#pragma once

#include <concepts>
#include <cstdint>
#include <random>
#include "engines/common/rng_linear_reduction.hpp"

namespace dumb_math::random::tests {

template <std::uniform_random_bit_generator generatorT>
uint64_t AccumPiPointsScalar(generatorT& engine, uint64_t points_num)
{
    uint64_t points_in_circle_num = 0;

    constexpr uint64_t r_max = generatorT::max();
    constexpr uint64_t r_sq = r_max * r_max;
    
    for (uint64_t i = 0; i < points_num; ++i)
    {
        float point_x = engine();
        float point_y = engine();
        
        float radius2 = point_x * point_x + point_y * point_y;

        if (radius2 <= r_sq)
            ++points_in_circle_num;
    }

    return points_in_circle_num;
}


template <std::uniform_random_bit_generator generatorT, std::floating_point CalcT>
CalcT PiMonteCarloScalarOneThread(uint64_t points_num)
{
    generatorT engine;

    uint64_t points_in_circle_num = AccumPiPointsScalar(engine, points_num);

    CalcT pi = 4 * static_cast<CalcT>(points_in_circle_num) / points_num;

    return pi;
}



} // namespace dumb_math::random::tests