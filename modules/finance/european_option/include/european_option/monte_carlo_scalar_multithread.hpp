#pragma once

#include "option_params_batch.hpp"
#include "rng_parallelizer/rng_parallelizer.hpp"
#include <atomic>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstddef>
#include <functional>

template <dumb_math::random::concepts::ScalarRngType GeneratorT>
class monte_carlo_batch_solver
{
public:
    explicit monte_carlo_batch_solver(std::size_t num_paths, unsigned int seed)
        : num_paths_(num_paths)
        , seed_(seed)
    {
    }

    std::vector<double> calculate_batch(const option_params_batch& batch, std::size_t num_options) const
    {
        // Для скалярного генератора dimension() == 1.
        // skipahead_step равен количеству путей, так как 1 путь = 1 вызов rng().
        const uint32_t skipahead_step = static_cast<uint32_t>(num_paths_);

        std::atomic<uint64_t> global_offset{0};

        std::function<std::vector<double>(GeneratorT&, uint64_t)> task = 
            [&](GeneratorT& rng, uint64_t count) -> std::vector<double>
        {
            uint64_t start_idx = global_offset.fetch_add(count, std::memory_order_relaxed);
            std::vector<double> results;
            results.reserve(count);

            for (uint64_t i = 0; i < count; ++i)
            {
                const uint64_t idx = start_idx + i;

                const double drift    = (batch.r[idx] - 0.5 * batch.sigma[idx] * batch.sigma[idx]) * batch.t[idx];
                const double vol      = batch.sigma[idx] * std::sqrt(batch.t[idx]);
                const double discount = std::exp(-batch.r[idx] * batch.t[idx]);

                double sum_payoffs = 0.0;
                std::normal_distribution<double> dist(0.0, 1.0);

                for (std::size_t j = 0; j < num_paths_; ++j)
                {
                    const double z = dist(rng);
                    const double s_t = batch.s0[idx] * std::exp(drift + vol * z);
                    sum_payoffs += std::max(s_t - batch.k[idx], 0.0);
                }
                
                results.push_back(discount * (sum_payoffs / num_paths_));
            }
            return results;
        };

        // Запуск параллелизатора: распределение 100 опционов по ядрам
        auto thread_data = dumb_math::random::RngParallelRun<GeneratorT>(
            num_options,
            skipahead_step,
            seed_,
            task,
            0,
            {0, 1, 3}
        );

        std::vector<double> final_results;
        final_results.reserve(num_options);
        for (auto& vec : thread_data)
        {
            final_results.insert(final_results.end(), vec.begin(), vec.end());
        }
        return final_results;
    }

private:
    std::size_t num_paths_;
    unsigned int seed_;
};