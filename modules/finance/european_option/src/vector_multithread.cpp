#include "european_option/analytical.hpp"
#include "european_option/option_params.hpp"
#include "european_option/monte_carlo_vector_multithread.hpp" 
#include "engines/minstd/generator_vect.hpp"                // Путь к векторному MinstdAVX2
#include "benchmarking/benchmarking.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

void print_report(const option_params& params, std::size_t num_paths, unsigned int seed, double mc_price, double bs_price)
{
    const double abs_err = std::abs(bs_price - mc_price);
    const double rel_err = (abs_err / bs_price) * 100.0;

    std::cout << "=== EXECUTION REPORT (First Option in Batch) ===\n"
              << "[Market & Instrument Data]\n"
              << std::fixed << std::setprecision(5)
              << "  Spot Price (s0)          : " << params.s0 << "\n"
              << "  Strike Price (k)         : " << params.k << "\n"
              << "  Risk-free Rate (r)       : " << params.r << "\n"
              << "  Volatility (sigma)       : " << params.sigma << "\n"
              << "  Time to Maturity (t)     : " << params.t << "\n\n"
              << "[Simulation Configuration]\n"
              << "  Path Count (N)           : " << num_paths << "\n"
              << "  RNG Seed                 : " << seed << "\n\n"
              << "[Pricing Results]\n"
              << "  Monte Carlo Price        : " << mc_price << "\n"
              << "  Analytical BS Price      : " << bs_price << "\n"
              << "  Absolute Error           : " << abs_err << "\n"
              << "  Relative Error (%)       : " << rel_err << "%\n";
}

int main()
{
    const std::size_t num_options = 100;
    const std::size_t num_paths = 100000;
    const unsigned int seed = 2125306575;

    option_params batch0;
    option_params_batch batch;

    std::mt19937 param_rng(54321);
    std::uniform_real_distribution<double> dist_rate(0.01, 0.15);     // r: 1% - 15%
    std::uniform_real_distribution<double> dist_vol(0.05, 0.80);      // sigma: 5% - 80%
    std::uniform_real_distribution<double> dist_price(50.0, 150.0);   // s0, k: 50 - 150
    std::uniform_real_distribution<double> dist_time(0.1, 3.0);       // t: 0.1 - 3 года

    for (std::size_t i = 0; i < num_options; ++i)
    {
        option_params rand_params
        {
            dist_rate(param_rng),
            dist_vol(param_rng),
            dist_price(param_rng),
            dist_price(param_rng),
            dist_time(param_rng)
        };
        batch.push_back(rand_params);

        if (i == 0) batch0 = rand_params;
    }

    analytical_solver bs_solver;
    const double bs_price = bs_solver.calculate_call(batch0);

    monte_carlo_batch_solver_avx2<dumb_math::random::engines::MinstdAVX2> mc_solver(num_paths, seed);
    
    std::vector<double> mc_prices = mc_solver.calculate_batch(batch, num_options);

    print_report(batch0, num_paths, seed, mc_prices[0], bs_price);

    using namespace dumb_math::benchmarking;

    ResultT mc_perf_results = TestThroughput([&]()
    {
        mc_prices = mc_solver.calculate_batch(batch, num_options);
    }, 10, 3);

    std::cout << "\n=== MONTE CARLO PERFORMANCE DATA (BATCH AVX2) ===\n"
              << "Total Options Processed: " << num_options << "\n"
              << "Average Throughput     : " << std::fixed << std::setprecision(2) << mc_perf_results.average << " tacts\n"
              << "Standard Deviation     : " << mc_perf_results.standard_deviation << " tacts\n";

    return 0;
}