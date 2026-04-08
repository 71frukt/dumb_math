#include "two_level_test/two_level_tester.hpp"

#include "uniformity_tests/iuniformity_tester.hpp"
#include "utils/bounds_check.hpp"

#include <cstddef>
#include <iostream>
#include <vector>
#include <span>

namespace rnd_generators_test {

double TwoLevelTester::Run(const IFirstLevelTester& first_lvl_tester, const IUniformityTester& u_tester, size_t block_count)
{
    std::vector<double> test_statistics;

    std::vector<double> cdf_values;
    cdf_values.reserve(block_count);

    for (size_t i = 0; i < block_count; i++)
    {
        const std::span block = GetBlock_(i, block_count);
        double stat_i = first_lvl_tester.GetTestStatistics(block);

        double cdf_i = first_lvl_tester.GetCDF(stat_i);
        cdf_values.push_back(cdf_i);
    }

    return u_tester.Run(cdf_values);
}


std::span<const double> TwoLevelTester::GetBlock_(size_t block_index, const size_t block_count) const
{
    assert(block_index < block_count);

    const size_t block_size = sequence_.size() / block_count;
    const size_t remainder  = sequence_.size() % block_count;

    const size_t offset = (block_index < remainder)
    ? block_index * (block_size + 1)
    : block_index * block_size + remainder;

    const size_t current_block_size = (block_index < remainder)
    ? (block_size + 1)
    : block_size;

    return std::span<const double>(sequence_.data() + offset, current_block_size);
}

} // namespace rnd_generators_test