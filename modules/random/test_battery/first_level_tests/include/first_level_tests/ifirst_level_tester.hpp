#pragma once

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>
#include <span>

#include "RLogSU/logger.hpp"

namespace rnd_generators_test {

class IFirstLevelTester
{
public:
    [[nodiscard]] virtual double GetTestStatistics(std::span<const double> block) const = 0;
    [[nodiscard]] virtual double GetCDF           (double statistic_y)            const = 0;
    
};

} // namespace rnd_generators_test