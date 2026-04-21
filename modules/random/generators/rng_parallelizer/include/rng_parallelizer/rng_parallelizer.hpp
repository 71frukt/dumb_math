#include <unistd.h>
#include <cstdint>
#include <concepts>

namespace dumb_math::random {

template <typename RngType, typename TaskFunc>
requires requires(TaskFunc task, RngType& rng, uint64_t count) {
    { task(rng, count) } -> std::default_initializable;
}
auto RngParallelRun(uint64_t total_elements, uint32_t seed, TaskFunc task);

} // namespace dumb_math::random

#include "impl/rng_parallelizer.inl"