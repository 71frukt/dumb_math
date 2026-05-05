#pragma once
#include "option_params.hpp"
#include <cstddef>
#include <cmath>
#include <random>
#include <algorithm>

class monte_carlo_solver
{
public:
    explicit monte_carlo_solver(std::size_t num_paths, unsigned int seed)
    : num_paths_(num_paths)
    , seed_(seed)
    {}

    double calculate_call(const option_params& params) const
    {
        std::mt19937_64 rng(seed_);; 
        std::normal_distribution<double> dist(0.0, 1.0);

        double drift = (params.r - 0.5 * params.sigma * params.sigma) * params.t;
        double vol = params.sigma * std::sqrt(params.t);
        double sum_payoffs = 0.0;

        for (std::size_t i = 0; i < num_paths_; ++i)
        {
            double z = dist(rng);
            double s_t = params.s0 * std::exp(drift + vol * z);
            sum_payoffs += std::max(s_t - params.k, 0.0);
        }

        double discount = std::exp(-params.r * params.t);
        return discount * (sum_payoffs / num_paths_);
    }

private:
    std::size_t num_paths_;
    unsigned int seed_;
};