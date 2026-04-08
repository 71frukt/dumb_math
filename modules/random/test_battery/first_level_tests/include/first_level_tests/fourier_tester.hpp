#include "first_level_tests/ifirst_level_tester.hpp"
#include <cmath>
#include <boost/math/distributions/normal.hpp>

namespace rnd_generators_test {

class FourierTester : public IFirstLevelTester
{
public:
    explicit FourierTester(size_t block_size_2_deg)
        : block_size_2_deg_(block_size_2_deg)
    {
        // block_size_2_deg должно быть степенью 2 для FFT
        assert((block_size_2_deg != 0) && ((block_size_2_deg & (block_size_2_deg - 1)) == 0));
    }

    [[nodiscard]] double GetTestStatistics(std::span<const double> block) const override;

    [[nodiscard]] double GetCDF(double statistic_y) const override;

private:
    size_t block_size_2_deg_;
};

} // namespace rnd_generators_test