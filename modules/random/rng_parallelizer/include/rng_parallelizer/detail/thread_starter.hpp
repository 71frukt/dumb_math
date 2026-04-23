#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <pthread.h>
#include <unistd.h>
#include <cstdint>
#include <sched.h>
#include <cstring>
#include <mutex>
#include <cerrno>
#include "../concepts.hpp"
#include "RLogSU/logger.hpp"


namespace dumb_math::random {
namespace detail {

template <concepts::RngType RngT, typename TaskFunc>
struct ThreadContext
{
    uint32_t seed;
    uint64_t n_elements;
    uint64_t offset;
    int      core_id;
    int      rt_priority;

    TaskFunc task;
    
    using ResultType = std::invoke_result_t<TaskFunc, RngT&, uint64_t>;
    static_assert(std::is_default_constructible_v<ResultType>, 
                  "Task return type must be default constructible");
                  
    ResultType result{};
};


inline std::once_flag sched_warning_eprem_flag;     // threads priority
inline std::once_flag sched_warning_einval_flag;    // invalid policy

template <concepts::RngType RngT, typename TaskFunc>
void* StartRngThreadOnCore(void* context)
{
    auto* thread_ctx = static_cast<ThreadContext<RngT, TaskFunc>*>(context);

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(thread_ctx->core_id, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    if (thread_ctx->rt_priority > 0)
    {
        struct sched_param param;
        std::memset(&param, 0, sizeof(param));
        param.sched_priority = thread_ctx->rt_priority;
        
        int rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);

        if (rc == EPERM)
        {
            std::call_once(sched_warning_eprem_flag, [rc]() {
                RLSU_WARNING("The program is running not in sudo mode, the configured thread priorities are ignored");
            });
        }

        else if (rc == EINVAL)
        {
            std::call_once(sched_warning_einval_flag, [rc]() {
                RLSU_WARNING("Invalid sched_priority");
            });
        }
    }

    RngT rng(thread_ctx->seed);
    rng.skipahead(thread_ctx->offset);

    thread_ctx->result = thread_ctx->task(rng, thread_ctx->n_elements);

    return nullptr;
}

} // namespace detail
} // namespace dumb_math::random