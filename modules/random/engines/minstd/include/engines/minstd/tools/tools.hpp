#pragma once

#include <cstdint>

namespace dumb_math::random {
namespace tools {

constexpr uint64_t MERSENNE_NUM = (0x1UL << 31) - 1;

uint32_t FastMersenneModExp(uint64_t base, uint64_t exp);

inline uint32_t MersenneMod(uint64_t x)
{
    uint64_t mod1 = (x    >> 31) + (x    & 0x7FFFFFFF);
    uint32_t mod2 = (mod1 >> 31) + (mod1 & 0x7FFFFFFF);

    return mod2;
}

} // namespace tools
} // namespace dumb_math::random
