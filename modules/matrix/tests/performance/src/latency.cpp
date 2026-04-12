#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <utility>
#include <vector>

#include "benchmarking/common.hpp"
#include "benchmarking/latency_test.hpp" 
#include "matrix/matrix.hpp"

#ifndef LATENCY_TESTS_DIR
#define LATENCY_TESTS_DIR "."
#endif

#ifndef PERFORMANCE_TEST_TIME_LIMIT_S
#define PERFORMANCE_TEST_TIME_LIMIT_S 10
#endif

template <typename Func>
void RunMatrixBenchmark(const std::string& test_name, Func mult_func) 
{
    using namespace dumb_math;

    using Point = std::pair<double, benchmarking::ResultT>;
    std::vector<Point> tests;

    double matrix_size = 2.0;
    std::chrono::duration<double> test_time_s = std::chrono::duration<double>::zero();

    while (test_time_s.count() < PERFORMANCE_TEST_TIME_LIMIT_S && matrix_size < 125)
    {
        auto start_s = std::chrono::steady_clock::now();

        matrix::Matrix matrix1    (matrix_size, matrix_size);
        matrix::Matrix matrix2    (matrix_size, matrix_size);
        matrix::Matrix matrix_dest(matrix_size, matrix_size);

        benchmarking::ResultT res = benchmarking::TestLatency([&]() {
            mult_func(matrix1, matrix2, matrix_dest); 
        }, 50, 30, 5);

        tests.push_back(Point(matrix_size, res));

        auto end_s   = std::chrono::steady_clock::now();
        test_time_s = end_s - start_s;
        matrix_size *= 2;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "[ BMARK    ] " << "[" << test_time_s.count() << " sec] " << test_name
                  << " Matrix size: " << matrix_size
                  << ", Latency: "    << res.average
                  << " cycles, CV: "  << (res.standard_deviation / res.average * 100.0) << "%"
                  << std::endl;

    }

    std::filesystem::path dir_path   = std::filesystem::path(LATENCY_TESTS_DIR);
    std::filesystem::path ofile_path = dir_path / (test_name + ".csv");

    benchmarking::ExportResultsToCSV(test_name, tests, ofile_path);
}

TEST(MatrixPerformance, DumbMul0)
{
    RunMatrixBenchmark("DumbMul0", dumb_math::matrix::Matrix::DumbMul0_);
}

TEST(MatrixPerformance, DumbMul1)
{
    RunMatrixBenchmark("DumbMul1", dumb_math::matrix::Matrix::DumbMul1_);
}

TEST(MatrixPerformance, OptMul0)
{
    RunMatrixBenchmark("OptMul0", dumb_math::matrix::Matrix::OptMul0_);
}

TEST(MatrixPerformance, OptMul1)
{
    RunMatrixBenchmark("OptMul1", dumb_math::matrix::Matrix::OptMul1_);
}

TEST(MatrixPerformance, BlockMul0)
{
    RunMatrixBenchmark("BlockMul0", dumb_math::matrix::Matrix::BlockMul0_);
}