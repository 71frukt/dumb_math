#pragma once

#include <vector>

namespace rnd_generators_test {

class IUniformityTester {
public:
    virtual ~IUniformityTester() = default;
    virtual double Run(std::vector<double>& u_values) const = 0;
};

} // namespace rnd_generators_test {
