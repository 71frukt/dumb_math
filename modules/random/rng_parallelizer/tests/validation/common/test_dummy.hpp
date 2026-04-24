#pragma once

#include <cstdint>

#include "rng_parallelizer/concepts.hpp"

namespace dumb_math::random::tests {

struct DummyRng
{
    explicit DummyRng(uint32_t seed) : seed(seed), offset(0) {}
    
    uint32_t seed = 0;
    uint64_t offset = 0;
    uint64_t operator()() { return offset++; }
    
    void skipahead(uint64_t steps) { offset += steps; }
    
    static constexpr uint64_t min() { return 0; }
    static constexpr uint64_t max() { return UINT64_MAX; }
};

static_assert(concepts::RngParallelizable<DummyRng>, "DummyRng does not satisfy RngParallelizable concept!");

struct TaskExecutionRecord
{
    uint64_t received_count;
    uint64_t received_offset;
};

struct DummyTask
{
    TaskExecutionRecord operator()(DummyRng& rng, uint64_t count) const {
        return {count, rng.offset};
    }
};

} // namespace dumb_math::random::tests
