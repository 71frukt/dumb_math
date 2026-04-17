#include <iostream>
#include <filesystem>

#include "benchmarking/benchmarking.hpp" 

using namespace dumb_math;

namespace {

__attribute__((noinline)) double DotProduct1(const double* a, const double* b, size_t n)
{
    double result = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        result += a[i] * b[i];
    }

    return result;
}

__attribute__((noinline)) double DotProduct2(const double* a, const double* b, size_t n)
{
    double result = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        result += a[i] * b[i];
        result += a[i] * b[i];
    }

    return result;
}

template <typename BenchFunc, typename TaskFunc>
void GeneratePlotData(BenchFunc bench_func, TaskFunc task_func, std::vector<size_t> sizes, std::string test_name, std::filesystem::path tests_dir)
{
    using Point = std::pair<double, benchmarking::ResultT>;
    std::vector<Point> tests;

    for (size_t N : sizes)
    {
        std::vector<double> a(N, 1.01);
        std::vector<double> b(N, 2.02);

        auto test_func = [&]() __attribute__((always_inline)) {
            return task_func(a.data(), b.data(), N);
        };

        auto res = bench_func(test_func, 100, 100, 100);

        std::cout << N << std::endl;

        tests.push_back({N, res});
    }

    std::filesystem::path dir_path   = tests_dir;
    std::filesystem::path ofile_path = dir_path / (test_name + ".csv");

    benchmarking::ExportResultsToCSV(test_name, tests, ofile_path);
}

} // namespace

int main()
{
    std::vector<size_t> sizes = {1, 2, 4, 8, 16, 32, 64, 128};
    
    auto latency_wrapper = [](auto&& func, size_t b_num, size_t b_size, size_t t_size) {
        return benchmarking::TestLatency(std::forward<decltype(func)>(func), b_num, b_size, t_size);
    };
    
    auto throwghput_wrapper = [](auto&& func, size_t b_num, size_t b_size, size_t t_size) {
        return benchmarking::TestThroughput(std::forward<decltype(func)>(func), b_num, b_size, t_size);
    };

    GeneratePlotData(latency_wrapper,    DotProduct1, sizes, "latency1",    std::filesystem::path(LATENCY_TESTS_DIR));
    GeneratePlotData(throwghput_wrapper, DotProduct1, sizes, "throwghput1", std::filesystem::path(THROUGHPUT_TESTS_DIR));

    GeneratePlotData(latency_wrapper,    DotProduct2, sizes, "latency2",    std::filesystem::path(LATENCY_TESTS_DIR));
    GeneratePlotData(throwghput_wrapper, DotProduct2, sizes, "throwghput2", std::filesystem::path(THROUGHPUT_TESTS_DIR));
}