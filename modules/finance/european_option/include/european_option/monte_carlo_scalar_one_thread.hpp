#include <random>
#include <cmath>
#include <algorithm>
#include <sched.h>
#include "european_option/option_params.hpp"
#include "rng_parallelizer/concepts.hpp"

template <dumb_math::random::concepts::ScalarRngType GeneratorT>
class monte_carlo_solver
{
public:
    monte_carlo_solver(std::size_t num_paths, unsigned int seed)
        : num_paths_(num_paths)
        , rng_(seed)
    {
    }

    double calculate_call(const option_params& params)
    {
        const double drift = (params.r - 0.5 * params.sigma * params.sigma) * params.t;
        const double vol_sqrt_t = params.sigma * std::sqrt(params.t);
        const double discount = std::exp(-params.r * params.t);

        std::normal_distribution<double> dist(0.0, 1.0);
        double sum_payoffs = 0.0;

        for (std::size_t i = 0; i < num_paths_; ++i)
        {
            const double z = dist(rng_);
            const double s_t = params.s0 * std::exp(drift + vol_sqrt_t * z);
            sum_payoffs += std::max(s_t - params.k, 0.0);
        }

        return discount * (sum_payoffs / static_cast<double>(num_paths_));
    }

private:
    std::size_t num_paths_;
    GeneratorT rng_;
};
