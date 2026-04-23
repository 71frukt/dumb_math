#include <cstdint>
#include "engines/minstd/tools/mersenne_mod_exp.hpp"

namespace dumb_math::random::engines {
namespace tools {

uint32_t FastMersenneModExp(uint64_t base, uint64_t exp)
{
    uint32_t multiplier = 1;

    while (exp > 0)
    {
        if (exp & 1)    // exp % 2 != 0
        {
            uint64_t mul = multiplier * base;
            multiplier = tools::MersenneMod(mul);
        }

        uint64_t base2 = base * base;
        base = tools::MersenneMod(base2);

        exp >>= 1;      // exp /= 2
    }

    return multiplier;
}


} // namespace tools
} // namespace dumb_math::random::engines
