#include "first_level_tests/disjoint_vectors.hpp"
#include "utils/bounds_check.hpp"
#include <cmath>
#include <numeric>
#include <algorithm>

namespace rnd_generators_test {

DisjointVectorsTester::DisjointVectorsTester(size_t dimension, size_t divisions_per_axis)
    : dimension_         (dimension)
    , divisions_per_axis_(divisions_per_axis)
    , total_bins_        (static_cast<size_t>(std::pow(divisions_per_axis, dimension)))
    , expected_dist_     (static_cast<double>(total_bins_) - 1.0)
{
    assert(dimension_ >= 1);
    assert(divisions_per_axis_ > 1);
}

double DisjointVectorsTester::GetTestStatistics(std::span<const double> block) const
{
    assert(utils::BoundsCheckStrict(block, 0, 1));

    const size_t total_vectors = block.size() / dimension_;
    if (total_vectors == 0) return 0.0;

    std::vector<double> observed(total_bins_, 0.0);

    for (size_t i = 0; i < total_vectors; ++i)
    {
        size_t bin_index = 0;
        size_t multiplier = 1;

        for (size_t d = 0; d < dimension_; ++d)
        {
            double val = block[i * dimension_ + d];
            size_t pos = std::min(static_cast<size_t>(val * divisions_per_axis_), divisions_per_axis_ - 1);
            
            bin_index += pos * multiplier;
            multiplier *= divisions_per_axis_;
        }

        observed[bin_index] += 1.0;
    }

    const double expected = static_cast<double>(total_vectors) / total_bins_;
    double y_statistic = 0.0;

    for (double obs : observed)
    {
        double diff = obs - expected;
        y_statistic += (diff * diff) / expected;
    }

    return y_statistic;
}

double DisjointVectorsTester::GetCDF(double statistic_y) const
{
    return boost::math::cdf(expected_dist_, statistic_y);
}

} // namespace rnd_generators_test