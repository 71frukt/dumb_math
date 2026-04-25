#pragma once

#include <cstdint>
#include "tools/mersenne_mod_exp.hpp"
#include "rng_parallelizer/concepts.hpp"

namespace dumb_math::random::engines {

class Minstd
{
public:
    explicit Minstd(uint32_t seed = 12345)
    {
        state_ = (seed == 0 || seed == tools::MERSENNE_NUM) ? 1 : tools::MersenneMod(seed);
    }

public:
    using result_t = uint32_t;
    
    static constexpr result_t min()       noexcept { return 1; }
    static constexpr result_t max()       noexcept { return tools::MERSENNE_NUM - 1; }
    static constexpr uint32_t dimension() noexcept { return 1; }

    result_t operator()()
    {
        uint64_t ax = a_ * state_;
        uint32_t ax_mod_m = tools::MersenneMod(ax);
        
        state_ = ax_mod_m;

        return ax_mod_m;
    }

    void skipahead(uint64_t offset)
    {
        uint64_t multiplier = tools::FastMersenneModExp(a_, offset);

        uint64_t new_state = state_ * multiplier;
        state_ = tools::MersenneMod(new_state);
    }

private:
    static constexpr uint64_t a_ = 48271;
    uint64_t state_;
};

static_assert(concepts::ScalarRngType<Minstd>, "DummyRng does not satisfy RngParallelizable concept!");


} // namespace dumb_math::random::engines