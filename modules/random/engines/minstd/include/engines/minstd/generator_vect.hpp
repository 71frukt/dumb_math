#pragma once

#include <cstdint>
#include <immintrin.h>
#include "rng_parallelizer/concepts.hpp"

#include "tools/mersenne_mod_exp.hpp"

namespace dumb_math::random::engines {

class MinstdAVX2
{
public:
    explicit MinstdAVX2(uint32_t seed = 12345);

public:

    using result_t = __m256i;
    
    static constexpr uint32_t min() noexcept { return 1; }
    static constexpr uint32_t max() noexcept { return tools::MERSENNE_NUM - 1; }

    result_t operator()();

    void skipahead(uint64_t offset);

private:
    static constexpr uint64_t a_ = 48271;
    
    __m256i state_vec_;
    __m256i a_vec_;

    // state * mult mod M
    static inline __m256i StateMulMod(__m256i state, __m256i mult);
};


static_assert(concepts::SimdRngType<MinstdAVX2>, "DummyRng does not satisfy RngParallelizable concept!");

} // namespace dumb_math::random::engines