#pragma once

#include <cstdint>
#include "tools/mersenne_mod_exp.hpp"

namespace dumb_math::random::engines {

class MinstdVect
{
public:
    explicit MinstdVect(uint32_t seed = 12345)
    {
        state_ = (seed == 0 || seed == tools::MERSENNE_NUM) ? 1 : tools::MersenneMod(seed);
    }

public:
    using result_t = uint32_t;
    
    static constexpr result_t min() { return 1; }
    static constexpr result_t max() { return tools::MERSENNE_NUM - 1; }

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


} // namespace dumb_math::random::engines