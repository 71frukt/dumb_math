#pragma once

#include "RLogSU/logger.hpp"
#include "rnd_normal_generator/rnd_normal_generator.hpp"
#include <random>
#include <stdexcept>

namespace dumb_math::random {

class ChiSqGenerator
{
public:
    explicit ChiSqGenerator(double chi_freedom_degree)
        : freedom_degree(chi_freedom_degree)
    {
        if (freedom_degree < 1.0)
        {
            RLSU_THROW<std::runtime_error>("degree of freedom < 1 in ChiGenerator ctor");
        }

        if (freedom_degree >= 2.0)
        {
            d_ = (freedom_degree / 2.0) - (1.0 / 3.0);
            c_ = 1.0 / std::sqrt(9.0 * d_);
        }
    }

    template <std::uniform_random_bit_generator generatorT>
    double operator()(generatorT& generator)
    {
        if (freedom_degree == 1.0)
        {
            double rnd_val = normal_generator_(generator);
            return rnd_val * rnd_val;
        }

        while (true)
        {
            double x = normal_generator_(generator);

            double v = 1.0 + c_ * x;

            if (v <= 0.0)
                continue;

            v = v * v * v;
            double u = NormalGenerator::ToCanonicalStrict(generator);
            double x2 = x * x;
            
            if (u < 1.0 - 0.0331 * x2 * x2)
                return 2.0 * d_ * v;

            if (std::log(u) < (x2 / 2.0) + d_ - d_ * v + d_ * std::log(v))
                return 2.0 * d_ * v;
        }
    }

    const double freedom_degree;
    
private:
    NormalGenerator normal_generator_;
    
    double c_ = 0;
    double d_ = 0;
    
};

} // namespace chi_sq_generator