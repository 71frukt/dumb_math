#pragma once

#include <immintrin.h>
#include <atomic>
#include <vector>
#include <cmath>
#include <functional>

#include "european_option/option_params_batch.hpp"
#include "rng_parallelizer/rng_parallelizer.hpp"

extern "C" 
{
    __m256d _ZGVdN4v_exp(__m256d);
    __m256d _ZGVdN4v_log(__m256d);
    __m256d _ZGVdN4v_sin(__m256d);
    __m256d _ZGVdN4v_cos(__m256d);
}

template <dumb_math::random::concepts::SimdRngType GeneratorT>
class monte_carlo_batch_solver_avx2
{
public:
    explicit monte_carlo_batch_solver_avx2(std::size_t num_paths, unsigned int seed)
        : num_paths_(num_paths)
        , seed_(seed)
    {
    }

    std::vector<double> calculate_batch(const option_params_batch& batch, std::size_t num_options) const
    {
        const uint32_t skipahead_step = static_cast<uint32_t>(num_paths_ / GeneratorT::dimension());

        std::atomic<uint64_t> global_offset{0};
        std::vector<double> final_results(num_options, 0.0);

        std::function<std::vector<double>(GeneratorT&, uint64_t)> task = 
            [&](GeneratorT& rng, uint64_t count) -> std::vector<double>
        {
            uint64_t start_idx = global_offset.fetch_add(count, std::memory_order_relaxed);

            for (uint64_t i = 0; i < count; ++i)
            {
                const uint64_t idx = start_idx + i;
                if (idx >= num_options) break;
                
                final_results[idx] = price_option_avx2(batch, idx, rng);
            }
            
            return {}; 
        };

        auto thread_data = dumb_math::random::RngParallelRun<GeneratorT>(
            num_options,
            skipahead_step,
            seed_,
            task,
            0,
            {}
        );
        
        return final_results;
    }

private:
    double price_option_avx2(const option_params_batch& batch, uint64_t idx, GeneratorT& rng) const
    {
        const double drift = (batch.r[idx] - 0.5 * batch.sigma[idx] * batch.sigma[idx]) * batch.t[idx];
        const double vol_sqrt_t = batch.sigma[idx] * std::sqrt(batch.t[idx]);
        const double discount = std::exp(-batch.r[idx] * batch.t[idx]);

        const __m256d v_drift      = _mm256_set1_pd(drift);
        const __m256d v_vol_sqrt_t = _mm256_set1_pd(vol_sqrt_t);
        const __m256d v_s0         = _mm256_set1_pd(batch.s0[idx]);
        const __m256d v_k          = _mm256_set1_pd(batch.k[idx]);
        const __m256d v_zero       = _mm256_setzero_pd();
        
        const __m256d v_inv_m      = _mm256_set1_pd(1.0 / 2147483647.0);
        
        const __m256d v_min_norm   = _mm256_set1_pd(2.2250738585072014e-308);

        __m256d v_sum_payoffs      = _mm256_setzero_pd();

        const std::size_t aligned_paths = (num_paths_ / 8) * 8;
        if (aligned_paths == 0)
        {
            return 0.0;
        }

        for (std::size_t j = 0; j < aligned_paths; j += 8)
        {
            __m256i v_raw = rng(); 

            __m128i v_low_128  = _mm256_castsi256_si128(v_raw);
            __m128i v_high_128 = _mm256_extracti128_si256(v_raw, 1);

            __m256d v_u1 = _mm256_cvtepi32_pd(v_low_128);
            __m256d v_u2 = _mm256_cvtepi32_pd(v_high_128);

            v_u1 = _mm256_mul_pd(v_u1, v_inv_m);
            v_u2 = _mm256_mul_pd(v_u2, v_inv_m);

            v_u1 = _mm256_max_pd(v_u1, v_min_norm);

            __m256d v_z1, v_z2;
            box_muller_avx2(v_u1, v_u2, v_z1, v_z2);

            v_sum_payoffs = _mm256_add_pd(v_sum_payoffs, process_4_paths(v_z1, v_drift, v_vol_sqrt_t, v_s0, v_k, v_zero));
            v_sum_payoffs = _mm256_add_pd(v_sum_payoffs, process_4_paths(v_z2, v_drift, v_vol_sqrt_t, v_s0, v_k, v_zero));
        }

        alignas(32) double res_buf[4];
        _mm256_store_pd(res_buf, v_sum_payoffs);
        const double total_sum = res_buf[0] + res_buf[1] + res_buf[2] + res_buf[3];

        return discount * (total_sum / static_cast<double>(aligned_paths));
    }

    static inline __m256d process_4_paths(__m256d v_z, __m256d v_drift, __m256d v_vol, __m256d v_s0, __m256d v_k, __m256d v_zero)
    {
        __m256d v_arg = _mm256_fmadd_pd(v_vol, v_z, v_drift);
        __m256d v_st = _mm256_mul_pd(v_s0, v_exp_pd(v_arg));
        return _mm256_max_pd(_mm256_sub_pd(v_st, v_k), v_zero);
    }

    static inline void box_muller_avx2(__m256d v_u1, __m256d v_u2, __m256d& v_z1, __m256d& v_z2)
    {
        __m256d v_r = _mm256_sqrt_pd(_mm256_mul_pd(_mm256_set1_pd(-2.0), v_log_pd(v_u1)));
        __m256d v_theta = _mm256_mul_pd(_mm256_set1_pd(2.0 * 3.14159265358979323846), v_u2);

        v_z1 = _mm256_mul_pd(v_r, v_cos_pd(v_theta));
        v_z2 = _mm256_mul_pd(v_r, v_sin_pd(v_theta));
    }

    static inline __m256d v_exp_pd(__m256d x)
    {
        return _ZGVdN4v_exp(x);
    }

    static inline __m256d v_log_pd(__m256d x)
    {
        return _ZGVdN4v_log(x);
    }

    static inline __m256d v_sin_pd(__m256d x)
    {
        return _ZGVdN4v_sin(x);
    }

    static inline __m256d v_cos_pd(__m256d x)
    {
        return _ZGVdN4v_cos(x);
    }

    std::size_t num_paths_;
    unsigned int seed_;
};