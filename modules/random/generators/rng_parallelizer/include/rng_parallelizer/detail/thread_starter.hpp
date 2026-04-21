#pragma once

#define _GNU_SOURCE

#include <pthread.h>
#include <unistd.h>
#include <cstdint>
#include <sched.h>
#include <type_traits>

namespace dumb_math::random {
namespace detail {

template <typename RngType, typename TaskFunc>
struct ThreadContext
{
    uint32_t seed;
    uint64_t n_elements;
    uint64_t offset;
    int      core_id;
    
    TaskFunc task;
    
    using ResultType = std::invoke_result_t<TaskFunc, RngType&, uint64_t>;
    static_assert(std::is_default_constructible_v<ResultType>, 
                  "Task return type must be default constructible");
                  
    ResultType result{};
};

template <typename RngType, typename TaskFunc>
void* StartRngThreadOnCore(void* context)
{
    auto* thread_ctx = static_cast<ThreadContext<RngType, TaskFunc>*>(context);

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(thread_ctx->core_id, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    RngType rng;
    rng(thread_ctx->seed);
    rng.skipahead(thread_ctx->offset);

    thread_ctx->result = thread_ctx->task(rng, thread_ctx->n_elements);

    return nullptr;
}

} // namespace detail
} // namespace dumb_math::random