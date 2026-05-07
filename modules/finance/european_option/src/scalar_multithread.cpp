#include "european_option/analytical.hpp"
#include "european_option/option_params.hpp"
#include "european_option/monte_carlo_scalar_multithread.hpp"
#include "engines/minstd/generator_scalar.hpp"
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

    const option_params ref_params
    {
        0.05,  // r
        0.1,   // sigma
        100.0, // s0
        100.0, // k
        1.0    // t
    };

    option_params_batch batch;
    for (std::size_t i = 0; i < num_options; ++i)
    {
        batch.push_back(ref_params);
    }

    analytical_solver bs_solver;
    const double bs_price = bs_solver.calculate_call(ref_params);

    monte_carlo_batch_solver<dumb_math::random::engines::Minstd> mc_solver(num_paths, seed);
    
    std::vector<double> mc_prices = mc_solver.calculate_batch(batch, num_options);

    print_report(ref_params, num_paths, seed, mc_prices[0], bs_price);

    using namespace dumb_math::benchmarking;

    ResultT mc_perf_results = TestThroughput([&]()
    {
        mc_prices = mc_solver.calculate_batch(batch, num_options);
    }, 10, 3);

    std::cout << "\n=== MONTE CARLO PERFORMANCE DATA (BATCH) ===\n"
              << "Total Options Processed: " << num_options << "\n"
              << "Average Throughput     : " << std::fixed << std::setprecision(2) << mc_perf_results.average << " tacts\n"
              << "Standard Deviation     : " << mc_perf_results.standard_deviation << " tacts\n";

    return 0;
}