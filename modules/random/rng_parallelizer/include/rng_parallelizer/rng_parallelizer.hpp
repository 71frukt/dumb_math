#pragma once

#include <unistd.h>
#include <cstdint>
#include <concepts>
#include <vector>
#include "concepts.hpp"

namespace dumb_math::random {

// for setting max priority 
/*
    int policy = SCHED_FIFO;
    struct sched_param param;
    std::memset(&param, 0, sizeof(param));
    
    param.sched_priority = sched_get_priority_max(policy);
*/
// and call RngParallelRun(... , ... , ... , &sched_priority);

template <concepts::RngType RngT, typename TaskFunc>
requires requires(TaskFunc task, RngT& rng, uint64_t count) {
    { task(rng, count) } -> std::default_initializable;
}
auto RngParallelRun(uint64_t total_elements, uint32_t engine_calls_per_elem, uint32_t seed, 
                    TaskFunc task, int rt_priority = 0, std::vector<int> target_cores = {});

} // namespace dumb_math::random

#include "impl/rng_parallelizer.inl"