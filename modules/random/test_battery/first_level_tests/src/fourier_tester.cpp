#include "first_level_tests/fourier_tester.hpp"
#include "utils/math/fft.hpp"
#include <cmath>
#include <algorithm>

namespace rnd_generators_test {

double FourierTester::GetTestStatistics(std::span<const double> block) const
{
    size_t n = std::min(block.size(), block_size_2_deg_);
    std::vector<std::complex<double>> f(n);

    for (size_t i = 0; i < n; ++i)
    {
        f[i] = std::complex<double>(block[i], 0.0);
    }

    utils::math::FFT(f);

    double h = std::sqrt(2.995732274 * n);
    size_t count = 0;
    
    for (size_t i = 1; i < n / 2; ++i) 
    {
        if (std::abs(f[i]) < h)
        {
            count++;
        }
    }

    return static_cast<double>(count);
}

double FourierTester::GetCDF(double statistic_y) const
{
    double n = static_cast<double>(block_size_2_deg_);
    double mu = 0.95 * n / 2.0;
    double sigma = std::sqrt(n * 0.95 * 0.05 / 4.0);

    boost::math::normal_distribution<double> dist(mu, sigma);
    return boost::math::cdf(dist, statistic_y);
}

} // namespace rnd_generators_test