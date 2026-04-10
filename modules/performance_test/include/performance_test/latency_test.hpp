#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

#include "common.hpp"
#include "performance_test/common.hpp"
#include "variance_calc/variance_calculator.hpp"

namespace dumb_math::performante_test {


template <typename Func>
[[nodiscard]] ResultT TestLatency(Func&& testing_func, const size_t buckets_num  = 1000,
                                                       const size_t buckets_size = 1000,
                                                       const size_t test_size    = 100)
{
    for (size_t i = 0; i < buckets_size; ++i)
    {
        RunAndProtect(testing_func);
    }

    std::vector<double> mins(buckets_num);
    uint64_t total_min_sum = 0;

    for (size_t bucket = 0; bucket < buckets_num; ++bucket)
    {
        uint64_t bucket_min = std::numeric_limits<uint64_t>::max();

        for (size_t test = 0; test < buckets_size; ++test)
        {
            uint64_t start = detail::StartTimer();

            for (size_t i = 0; i < test_size; ++i)
            {
                RunAndProtect(testing_func);
            }

            uint64_t end = detail::StopTimer();

            uint64_t cycles = end - start;
            
            if (cycles < bucket_min)
            {
                bucket_min = cycles;
            }
        }

        mins[bucket]   = bucket_min;
        total_min_sum += bucket_min;
    }

    ResultT result;
    result.average  = double(total_min_sum) / double(buckets_num) / double(test_size);
    result.variance = random::variance::OnePass(mins);

    return result;
}

} // namespace dumb_math::performante_test