#pragma once

#include <concepts>
#include <cstdint>
#include <random>
#include <vector>
#include <sched.h>
#include <cstring>

#include <immintrin.h>

#include "rng_parallelizer/concepts.hpp"
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

template <typename generatorT>
uint64_t AccumPiPointsVector(generatorT& engine, uint64_t points_num)
{
    uint64_t points_in_circle_num = 0;

    constexpr uint64_t r_max = generatorT::max();
    constexpr uint64_t r_sq = r_max * r_max;

    __m256i accum = _mm256_setzero_si256();
    __m256i r_sq_plus_1_vec64 = _mm256_set1_epi64x(r_sq + 1);

    // Обрабатываем по 4 точки за 1 векторную итерацию
    uint64_t vec_iterations = points_num / 4;
    uint64_t remainder = points_num % 4;

    for (uint64_t i = 0; i < vec_iterations; ++i)
    {
        __m256i v = engine();

        __m256i x2 = _mm256_mul_epu32(v, v);
        
        __m256i v_odd = _mm256_srli_epi64(v, 32);
        __m256i y2 = _mm256_mul_epu32(v_odd, v_odd);

        __m256i r2 = _mm256_add_epi64(x2, y2);

        __m256i mask = _mm256_cmpgt_epi64(r_sq_plus_1_vec64, r2);
        accum = _mm256_sub_epi64(accum, mask);
    }

    alignas(32) uint64_t results[4];
    _mm256_store_si256(reinterpret_cast<__m256i*>(results), accum);

    points_in_circle_num = results[0] + results[1] + results[2] + results[3];

    if (remainder > 0)
    {
        __m256i v_tail = engine();
        alignas(32) uint32_t arr[8];
        _mm256_store_si256(reinterpret_cast<__m256i*>(arr), v_tail);

        for (uint64_t i = 0; i < remainder; ++i)
        {
            uint64_t px = arr[i * 2];
            uint64_t py = arr[i * 2 + 1];
            if (px * px + py * py <= r_sq)
            {
                ++points_in_circle_num;
            }
        }
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
    int policy = SCHED_FIFO;
    struct sched_param param;
    std::memset(&param, 0, sizeof(param));
    param.sched_priority = sched_get_priority_max(policy);

    std::vector<uint32_t> points_in_circle_nums = RngParallelRun<generatorT>(points_num, 2, 12345, 
                                                                        AccumPiPointsScalar<generatorT>, param.sched_priority);
    
    uint64_t all_points_in_circle_num = 0;
    for (uint32_t res : points_in_circle_nums)
        all_points_in_circle_num += res;

    
    CalcT pi = 4 * static_cast<CalcT>(all_points_in_circle_num) / points_num;

    return pi;
}


template <concepts::SimdRngType generatorT, std::floating_point CalcT>
CalcT PiMonteCarloVectorOneThread(uint64_t points_num)
{
    generatorT engine;

    uint64_t points_in_circle_num = AccumPiPointsVector(engine, points_num);

    CalcT pi = 4.0 * static_cast<CalcT>(points_in_circle_num) / points_num;

    return pi;
}

template <concepts::SimdRngType generatorT, std::floating_point CalcT>
CalcT PiMonteCarloVectorMultiThread(uint64_t points_num)
{
    int policy = SCHED_FIFO;
    struct sched_param param;
    std::memset(&param, 0, sizeof(param));
    param.sched_priority = sched_get_priority_max(policy);

    auto points_in_circle_nums = RngParallelRun<generatorT>(
        points_num, 
        2, 
        12345, 
        AccumPiPointsVector<generatorT>, 
        param.sched_priority,
        {0, 1, 6}
    );
    
    uint64_t all_points_in_circle_num = 0;
    for (uint64_t res : points_in_circle_nums)
    {
        all_points_in_circle_num += res;
    }
    
    CalcT pi = static_cast<CalcT>(4.0) * static_cast<CalcT>(all_points_in_circle_num) / static_cast<CalcT>(points_num);

    return pi;
}

} // namespace dumb_math::random::engines::tests