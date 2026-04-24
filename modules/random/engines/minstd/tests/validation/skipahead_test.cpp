#include <gtest/gtest.h>
#include <cstdint>

#include "engines/minstd/generator_scalar.hpp"
#include "engines/minstd/tools/mersenne_mod_exp.hpp"

namespace dumb_math::random::tests {

using namespace dumb_math::random::engines;

class MinstdScalarSkipAheadParamTest : public ::testing::TestWithParam<uint64_t> {};

INSTANTIATE_TEST_SUITE_P(
    VariousSkipSteps,
    MinstdScalarSkipAheadParamTest,
    ::testing::Values(
        0,
        1,
        2,
        10,
        100,
        1'000,
        10'000,
        50'000,
        100'000
    )
);

TEST_P(MinstdScalarSkipAheadParamTest, EquivalenceToSequential)
{
    const uint64_t skip_steps = GetParam(); 

    Minstd rng_jumped;
    Minstd rng_sequential;

    rng_jumped.skipahead(skip_steps);

    for (uint64_t i = 0; i < skip_steps; ++i)
    {
        rng_sequential();
    }

    EXPECT_EQ(rng_jumped(), rng_sequential()) 
        << "[TEST_FAILED] Failed at skip_steps = " << skip_steps;
}


TEST_P(MinstdScalarSkipAheadParamTest, AdditiveSkips)
{
    Minstd rng_combined;
    Minstd rng_split;

    const uint64_t skip_steps = GetParam();

    rng_combined.skipahead(2 * skip_steps);

    rng_split.skipahead(skip_steps);
    rng_split.skipahead(skip_steps);

    EXPECT_EQ(rng_combined(), rng_split())
        << "[TEST_FAILED] Failed at additive_skips = " << skip_steps;;
}


TEST(MinstdSkipAhead, FullPeriodSkip)
{
    const uint64_t full_period = tools::MERSENNE_NUM - 1;

    Minstd rng_jumped;
    Minstd rng_control;

    rng_jumped.skipahead(full_period);

    // Состояние совершило полный круг и должно совпасть со стартовым
    EXPECT_EQ(rng_jumped(), rng_control());
}

} // namespace dumb_math::random::tests