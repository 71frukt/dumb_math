#include "european_option/option_params.hpp"
#include "european_option/analytical.hpp"
#include "european_option/monte_carlo.hpp"
#include "benchmarking/benchmarking.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>


void print_report(const option_params& params, std::size_t num_paths, unsigned int seed, double mc_price, double bs_price)
{
    const double abs_err = std::abs(bs_price - mc_price);
    const double rel_err = (abs_err / bs_price) * 100.0;

    std::cout << "=== EXECUTION REPORT ===\n"
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
    const option_params params
    {
        0.05,  // r [cite: 236]
        0.1,   // sigma [cite: 236]
        100.0, // s0 [cite: 236]
        100.0, // k [cite: 236]
        1.0    // t [cite: 236]
    };

    const std::size_t num_paths = 1000000; // Total trajectory count [cite: 116, 236]
    const unsigned int seed = 2125306575;

    // Calculate baseline using Black-Scholes analytical approach [cite: 16, 19]
    analytical_solver bs_solver;
    const double bs_price = bs_solver.calculate_call(params);

    // Execute Monte Carlo simulation based on analytical solution of the SDE [cite: 108, 114]
    monte_carlo_solver mc_solver(num_paths, seed);
    const double mc_price = mc_solver.calculate_call(params);

    print_report(params, num_paths, seed, mc_price, bs_price);

    using namespace dumb_math::benchmarking;

    ResultT perf_results = TestThroughput([&]()
    {
        mc_solver.calculate_call(params);
    }, 10, 3);

    std::cout << "\n=== MONTE CARLO PERFORMANCE DATA ===\n"
              << "Average Throughput: " << std::fixed << std::setprecision(2) << perf_results.average << " tacts\n"
              << "Standard Deviation: " << perf_results.standard_deviation << " tacts\n";

    return 0;
}