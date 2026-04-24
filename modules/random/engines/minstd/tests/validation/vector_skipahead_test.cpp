#include <gtest/gtest.h>
#include <cstdint>
#include <immintrin.h>

#include "engines/minstd/generator_scalar.hpp"
#include "engines/minstd/generator_vect.hpp"
#include "engines/minstd/tools/mersenne_mod_exp.hpp"

namespace dumb_math::random::tests {

using namespace dumb_math::random::engines;

inline void ExpectM256iEq(__m256i a, __m256i b, const std::string& context = "")
{
    alignas(32) uint32_t arr_a[8];
    alignas(32) uint32_t arr_b[8];
    
    _mm256_store_si256(reinterpret_cast<__m256i*>(arr_a), a);
    _mm256_store_si256(reinterpret_cast<__m256i*>(arr_b), b);

    for (int i = 0; i < 8; ++i)
    {
        EXPECT_EQ(arr_a[i], arr_b[i]) << context << " (Mismatch at channel " << i << ")";
    }
}


TEST(MinstdVectorCrossValidation, MatchesScalarSequential)
{
    const uint32_t seed = 42;
    MinstdAVX2 rng_vec(seed);
    Minstd rng_scalar(seed);

    // 3 векторных шага = 24 скалярных
    for (int step = 0; step < 3; ++step)
    {
        __m256i vec_result = rng_vec();
        
        alignas(32) uint32_t arr_vec[8];
        _mm256_store_si256(reinterpret_cast<__m256i*>(arr_vec), vec_result);

        for (int i = 0; i < 8; ++i)
        {
            EXPECT_EQ(arr_vec[i], rng_scalar()) 
                << "Mismatch at vector step " << step << ", channel " << i;
        }
    }
}


class MinstdVectorSkipAheadParamTest : public ::testing::TestWithParam<uint64_t> {};

INSTANTIATE_TEST_SUITE_P(
    VariousSkipSteps,
    MinstdVectorSkipAheadParamTest,
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

TEST_P(MinstdVectorSkipAheadParamTest, EquivalenceToSequential)
{
    const uint64_t skip_steps = GetParam(); 

    MinstdAVX2 rng_jumped;
    MinstdAVX2 rng_sequential;

    rng_jumped.skipahead(skip_steps);

    for (uint64_t i = 0; i < skip_steps; ++i)
    {
        rng_sequential();
    }

    ExpectM256iEq(rng_jumped(), rng_sequential(), 
                  "[TEST_FAILED] Failed at skip_steps = " + std::to_string(skip_steps));
}

TEST_P(MinstdVectorSkipAheadParamTest, AdditiveSkips)
{
    MinstdAVX2 rng_combined; 
    MinstdAVX2 rng_split;

    const uint64_t skip_steps = GetParam();

    rng_combined.skipahead(2 * skip_steps);

    rng_split.skipahead(skip_steps);
    rng_split.skipahead(skip_steps);

    ExpectM256iEq(rng_combined(), rng_split(), 
                  "[TEST_FAILED] Failed at additive_skips = " + std::to_string(skip_steps));
}

TEST(MinstdVectorSkipAhead, FullPeriodSkip)
{
    const uint64_t full_period = tools::MERSENNE_NUM - 1;

    MinstdAVX2 rng_jumped;
    MinstdAVX2 rng_control;

    rng_jumped.skipahead(full_period);

    ExpectM256iEq(rng_jumped(), rng_control(), "Failed on full period jump");
}

} // namespace dumb_math::random::tests