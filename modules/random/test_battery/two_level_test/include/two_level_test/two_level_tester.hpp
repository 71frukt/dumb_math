#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

#include "first_level_tests/ifirst_level_tester.hpp"
#include "uniformity_tests/iuniformity_tester.hpp"
#include "utils/bounds_check.hpp"

namespace rnd_generators_test {

class TwoLevelTester
{
public:
    TwoLevelTester(const std::vector<double> sequence)
        : sequence_(sequence)
    {}

    [[nodiscard]] double Run(const IFirstLevelTester& first_lvl_tester, const IUniformityTester& u_test, size_t block_count);

private:

std::span<const double> GetBlock_(size_t block_index, const size_t block_count) const;

private:

    const std::vector<double> sequence_;
};

} // namespace rnd_generators_test