#pragma once

#include <cmath>
#include "option_params.hpp"

class analytical_solver
{
public:
    double calculate_call(const option_params& params) const
    {
        double d1 = (std::log(params.s0 / params.k) + (params.r + 0.5 * params.sigma * params.sigma) * params.t) / (params.sigma * std::sqrt(params.t));
        double d2 = d1 - params.sigma * std::sqrt(params.t);
        
        auto norm_cdf = [](double x)
        {
            return 0.5 * std::erfc(-x / std::sqrt(2.0));
        };
        
        return params.s0 * norm_cdf(d1) - params.k * std::exp(-params.r * params.t) * norm_cdf(d2);
    }
};