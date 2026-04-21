#include <pthread.h>
#include <unistd.h>
#include <cstdint>
#include <sched.h>
#include <vector>
#include <stdexcept>
#include "RLogSU/logger.hpp"

#include "../detail/thread_starter.hpp"
#include "../detail/cores_info_getter.hpp"

namespace dumb_math::random {

template <typename RngType, typename TaskFunc>
requires requires(TaskFunc task, RngType& rng, uint64_t count) {
    { task(rng, count) } -> std::default_initializable;
}
auto RngParallelRun(uint64_t total_elements, uint32_t seed, TaskFunc task)
{
    std::vector<int> target_cores = detail::GetPhysicalPCores();
    RLSU_INFO("target_cores (pcores) = {}", target_cores);

    int available_p_cores = target_cores.size();

    int num_threads = (available_p_cores > 1) ? available_p_cores - 1 : 1;

    uint64_t n_per_thread = total_elements / num_threads;
    uint64_t remainder = total_elements % num_threads;

    using ResultType = typename detail::ThreadContext<RngType, TaskFunc>::ResultType;
    
    std::vector<pthread_t> threads(num_threads);
    std::vector<detail::ThreadContext<RngType, TaskFunc>> contexts(num_threads);
    std::vector<ResultType> final_results(num_threads);

    for (int i = 0; i < num_threads; ++i)
    {
        contexts[i].seed = seed;
        contexts[i].n_elements = n_per_thread + (i == num_threads - 1 ? remainder : 0);
        contexts[i].offset = i * n_per_thread;
        
        contexts[i].core_id = target_cores[i]; 
        contexts[i].task = task;
    }

    // ----------------------- Calb line ------------------------------------------------------
    
    int threads_created = 0;

    for (int i = 0; i < num_threads; ++i)
    {
        int rc = pthread_create(&threads[i], nullptr, 
                                detail::StartRngThreadOnCore<RngType, TaskFunc>, 
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