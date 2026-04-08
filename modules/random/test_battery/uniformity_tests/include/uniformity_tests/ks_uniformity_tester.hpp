#pragma once

#include "uniformity_tests/iuniformity_tester.hpp"
#include "utils/bounds_check.hpp"

#include <vector>
#include <algorithm>
#include <boost/math/distributions/kolmogorov_smirnov.hpp>

namespace rnd_generators_test {

class KSTester : public IUniformityTester
{
public:
    double Run(std::vector<double>& u_values) const override
    {
        assert(utils::BoundsCheckStrict(u_values, 0, 1));

        const size_t n = u_values.size();
        std::sort(u_values.begin(), u_values.end());

        double d_max = 0.0;

        for (size_t j = 1; j <= n; ++j)
        {
            double d_plus  = (static_cast<double>(j) / n) - u_values[j - 1];
            double d_minus = u_values[j - 1] - (static_cast<double>(j - 1) / n);
            d_max = std::max({d_max, d_plus, d_minus});
        }

        boost::math::kolmogorov_smirnov_distribution<double> dist(static_cast<double>(n));
        return 1.0 - boost::math::cdf(dist, d_max);
    }
};

} // namespace rnd_generators_test
