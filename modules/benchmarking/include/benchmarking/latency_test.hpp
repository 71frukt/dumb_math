#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>
#include <algorithm>
#include <pthread.h>

#include "common.hpp"
#include "benchmarking/common.hpp"
#include "variance_calc/variance_calculator.hpp"
#include <iostream>

namespace dumb_math::benchmarking {


template <typename Func>
[[nodiscard]] ResultT TestLatency(Func&& testing_func, const size_t buckets_num    = 100,
                                                       const size_t buckets_size   = 100,
                                                       const size_t test_size      = 10,
                                                       const double emissions_part = 0.05)
{
    detail::CheckCpuGovernor();
    
    for (size_t i = 0; i < buckets_size; ++i)
    {
        detail::RunAndProtect(testing_func);
    }

    std::vector<double> mins(buckets_num);

    for (size_t bucket = 0; bucket < buckets_num; ++bucket)
    {
        uint64_t bucket_min = std::numeric_limits<uint64_t>::max();

        for (size_t test = 0; test < buckets_size; ++test)
        {
            uint64_t start = detail::StartTimer();

            for (size_t i = 0; i < test_size; ++i)
            {
                detail::RunAndProtect(testing_func);
            }

            uint64_t end = detail::StopTimer();

            uint64_t cycles = (end - start) / test_size;
            
            if (cycles < bucket_min)
            {
                bucket_min = cycles;
            }
        }

        mins[bucket] = bucket_min;
        // std::cout << "bucket_min = " << bucket_min << std::endl;
    }

    std::sort(mins.begin(), mins.end());
    mins.resize(buckets_num * (1 - emissions_part));    // dump emissions
    
    double clean_average = 0.0;

    for (double val : mins)
    {
        clean_average += val;
    }

    clean_average /= mins.size();

    ResultT result;
    result.average = clean_average;
    result.standard_deviation = std::sqrt(random::variance_calc::OnePass(mins));

    return result;
}

} // namespace dumb_math::benchmarking