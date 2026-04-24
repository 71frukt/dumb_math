#include <gtest/gtest.h>
#include <cstdint>
#include <vector>

#include "common/test_dummy.hpp"
#include "common/test_cores_getter.hpp"
#include "rng_parallelizer/rng_parallelizer.hpp"

namespace dumb_math::random::tests {

class RngParallelizerParamTest : public ::testing::TestWithParam<uint64_t> {};

INSTANTIATE_TEST_SUITE_P(
    TotalElemsSizes,
    RngParallelizerParamTest,
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

TEST_P(RngParallelizerParamTest, DynamicChunkingAndOffsets)
{
    const uint64_t total_elements = GetParam();
    DummyTask task;

    std::vector<int> cores = GetCores();

    auto results = RngParallelRun<DummyRng>(total_elements, 1, 12345, task, 0, cores);

    ASSERT_EQ(results.size(), cores.size());

    uint64_t real_sum = 0;
    uint64_t current_real_offset = 0;

    for (size_t i = 0; i < results.size(); ++i)
    {
        // каждый следующий сдвинут на размер предыдущего чанка
        EXPECT_EQ(results[i].received_offset, current_real_offset) 
            << "Offset mismatch at thread index " << i;

        real_sum += results[i].received_count;
        current_real_offset += results[i].received_count;
    }

    // все элементы обработаны
    EXPECT_EQ(real_sum, total_elements);
}

} // namespace dumb_math::random::tests