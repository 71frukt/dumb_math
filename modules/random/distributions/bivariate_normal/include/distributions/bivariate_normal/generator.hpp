#pragma once

#include <concepts>
#include <random>
#include <utility>
#include <cmath>

#include "rnd_normal_generator/rnd_normal_generator.hpp"

namespace dumb_math::random {

class BivariateNormalGenerator
{
public:
    explicit BivariateNormalGenerator(double mu_x, double mu_y, double sigma_x, double sigma_y, double rho)
        : mu_x_(mu_x), mu_y_(mu_y), sigma_x_(sigma_x), sigma_y_(sigma_y), rho_(rho)
    {
        sqrt_1_minus_rho_sq_ = std::sqrt(1.0 - rho_ * rho_);
    }

public:
    template <std::uniform_random_bit_generator generatorT>
    std::pair<double, double> operator()(generatorT& generator)
    {
        double z1 = normal_generator_(generator);
        double z2 = normal_generator_(generator);

        double x = mu_x_ + sigma_x_ * z1;
        double y = mu_y_ + sigma_y_ * (rho_ * z1 + sqrt_1_minus_rho_sq_ * z2);

        return {x, y};
    }

private:
    NormalGenerator normal_generator_;

    const double mu_x_;
    const double mu_y_;

    const double sigma_x_;
    const double sigma_y_;
    
    const double rho_;
    
    double sqrt_1_minus_rho_sq_;
};

} // namespace dumb_math::random