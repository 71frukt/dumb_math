#pragma once

#include <pthread.h>
#include <unistd.h>
#include <cstdint>
#include <sched.h>
#include <vector>
#include <stdexcept>
#include "RLogSU/logger.hpp"

#include "../concepts.hpp"
#include "../detail/thread_starter.hpp"
#include "../detail/cores_info_getter.hpp"

namespace dumb_math::random {

template <concepts::RngParallelizable RngT, typename TaskFunc>
requires requires(TaskFunc task, RngT& rng, uint64_t count) {
    { task(rng, count) } -> std::default_initializable;
}
auto RngParallelRun(uint64_t total_elements, uint32_t skipahead_step, uint32_t seed, 
                    TaskFunc task, int rt_priority, std::vector<int> target_cores)
{
    int available_p_cores = 0;
    int num_threads = 0;

    if (target_cores.empty())
    {
        target_cores = detail::GetPhysicalPCores();
        RLSU_INFO("target_cores (pcores) = {}", target_cores);
        
        available_p_cores = target_cores.size();
        num_threads       = (available_p_cores > 1) ? available_p_cores - 1 : 1;
    }

    else
    {
        available_p_cores = target_cores.size();
        num_threads       = available_p_cores;
    }


    uint64_t dimension = RngT::dimension();
    uint64_t period = std::lcm(dimension, skipahead_step) / skipahead_step; 

    uint64_t raw_n_per_thread = total_elements / num_threads;
    uint64_t n_per_thread = (raw_n_per_thread / period) * period;
    uint64_t remainder = total_elements - (n_per_thread * num_threads);

    RLSU_ASSERT((n_per_thread * skipahead_step) % dimension == 0);

    using ResultType = typename detail::ThreadContext<RngT, TaskFunc>::ResultType;
    
    std::vector<pthread_t> threads(num_threads);
    std::vector<detail::ThreadContext<RngT, TaskFunc>> contexts(num_threads);
    std::vector<ResultType> final_results(num_threads);

    for (int i = 0; i < num_threads; ++i)
    {
        contexts[i].seed = seed;
        contexts[i].n_elements = n_per_thread + (i == num_threads - 1 ? remainder : 0);
        contexts[i].offset = (i * n_per_thread * skipahead_step) / RngT::dimension();
        
        contexts[i].core_id = target_cores[i];
        contexts[i].rt_priority = rt_priority;
        contexts[i].task = task;
    }

    // ----------------------- Calb line ------------------------------------------------------
    
    int threads_created = 0;

    for (int i = 0; i < num_threads; ++i)
    {
        // TODO: тут каждый раз skipahead считается заново,
        // хотя следующий поток может использовать начальное значение предыдущего
        int rc = pthread_create(&threads[i], nullptr, 
                                detail::StartRngThreadOnCore<RngT, TaskFunc>, 
                                &contexts[i]);
        if (rc != 0)
            break; 

        threads_created++;
    }

    for (int i = 0; i < threads_created; ++i)
    {
        pthread_join(threads[i], nullptr);
        final_results[i] = std::move(contexts[i].result);
    }

    if (threads_created < num_threads)
        throw std::runtime_error("OS failed to allocate required number of pthreads");

    return final_results;
}

} // namespace dumb_math::random