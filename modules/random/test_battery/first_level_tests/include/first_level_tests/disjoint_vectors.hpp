#pragma once

#include "first_level_tests/ifirst_level_tester.hpp"
#include <boost/math/distributions/chi_squared.hpp>
#include <cstddef>
#include <vector>
#include <span>

namespace rnd_generators_test {

class DisjointVectorsTester : public IFirstLevelTester
{
public:
    DisjointVectorsTester(size_t dimension, size_t divisions_per_axis);

    [[nodiscard]] double GetTestStatistics(std::span<const double> block) const override;
    [[nodiscard]] double GetCDF(double statistic_y) const override;

private:
    const size_t dimension_;            // t
    const size_t divisions_per_axis_;   // d
    const size_t total_bins_;           // k = d^t
    const boost::math::chi_squared expected_dist_;
};

} // namespace rnd_generators_test