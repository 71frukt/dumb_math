#pragma once

#include <concepts>
#include <cstdint>
#include <random>
#include <vector>

#include "rng_parallelizer/rng_parallelizer.hpp"

namespace dumb_math::random::engines::tests {

template <std::uniform_random_bit_generator generatorT>
uint32_t AccumPiPointsScalar(generatorT& engine, uint32_t points_num)
{
    uint32_t points_in_circle_num = 0;

    constexpr uint64_t r_max = generatorT::max();
    constexpr uint64_t r_sq = r_max * r_max;
    
    for (uint32_t i = 0; i < points_num; ++i)
    {
        uint64_t point_x = engine();
        uint64_t point_y = engine();
        
        uint64_t radius2 = point_x * point_x + point_y * point_y;

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

    CalcT pi = 4.0 * static_cast<CalcT>(points_in_circle_num) / points_num;

    return pi;
}


template <std::uniform_random_bit_generator generatorT, std::floating_point CalcT>
CalcT PiMonteCarloScalarMultiThread(uint64_t points_num)
{
    std::vector<uint32_t> points_in_circle_nums = RngParallelRun<generatorT>(points_num, 2, 12345, AccumPiPointsScalar<generatorT>);
    
    uint64_t all_points_in_circle_num = 0;
    for (uint32_t res : points_in_circle_nums)
        all_points_in_circle_num += res;

    
    CalcT pi = 4 * static_cast<CalcT>(all_points_in_circle_num) / points_num;

    return pi;
}



} // namespace dumb_math::random::engines::tests