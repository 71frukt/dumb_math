#include <iostream>

#include "benchmarking/common.hpp"
#include "engines/minstd/generator_scalar.hpp"
#include "engines/common/pi_calculate.hpp"
#include "benchmarking/benchmarking.hpp"

using namespace dumb_math;

int main(int argc, char* argv[])
{
    uint64_t iterations = 100'000'000;

    if (argc > 1)
    {
        try 
        {
            iterations = std::stoull(argv[1]);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Invalid iterations argument: " << argv[1] << "\n";
            return 1;
        }
    }

    float pi = 0;

    auto calc_pi_func = [&]() {
        pi = random::engines::tests::PiMonteCarloScalarMultiThread<random::engines::Minstd, float>(iterations);
    };    

    benchmarking::ResultT bench_res = benchmarking::TestThroughput(calc_pi_func,
                                                                     3,
                                                                     1,
                                                                       1);

    std::cout << "[Throughput: scalar, multi thread]"   << std::endl
              << "pi = " << pi                          << std::endl
              << "clocs = " << bench_res.average
              << " +- " << bench_res.standard_deviation << std::endl
                                                        << std::endl;

    return 0;
}