#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <stdexcept>
#include <utility>
#include <vector>

#include "RLogSU/logger.hpp"
#include "benchmarking/common.hpp"
#include "benchmarking/latency_test.hpp" 
#include "matrix/matrix.hpp"
#include "assets/config.hpp"
#include "common/matrix_iostream.hpp"

#include "rnd_bit_generators/mrg32k3a.hpp"

#ifndef LATENCY_TESTS_DIR
#define LATENCY_TESTS_DIR "."
#endif

#ifndef PERFORMANCE_TEST_TIME_LIMIT_S
#define PERFORMANCE_TEST_TIME_LIMIT_S 10
#endif

namespace dumb_math::matrix::tests::performance {

static Matrix RequestMatrix(const size_t rows, const size_t cols, const size_t num_in_block)
{
    using namespace tests::iostream;

    const auto& matr_info = assets::config::RequestMatrix(rows, cols, num_in_block);

    std::ifstream matrix_file(matr_info.filepath);
    if (!matrix_file)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("no matrix [{}] in shop!", matr_info.filepath.c_str()));
    
    Matrix matrix(matr_info.rows, matr_info.cols);

    matrix_file >> matrix;
    if (!matrix_file)
        RLSU_THROW<std::runtime_error>("error parsing matrix");

    return matrix;
}

template <typename Func>
void RunMatrixBenchmark(const std::string& test_name, Func mult_func) 
{
    using namespace dumb_math;

    using Point = std::pair<double, benchmarking::ResultT>;
    std::vector<Point> tests;

    size_t matrix_size = 2;
    std::chrono::duration<double> test_time_s = std::chrono::duration<double>::zero();

    while (test_time_s.count() < PERFORMANCE_TEST_TIME_LIMIT_S)
    {
        auto start_s = std::chrono::steady_clock::now();

        matrix::Matrix matrix1 = RequestMatrix(matrix_size, matrix_size, 0);
        matrix::Matrix matrix2 = RequestMatrix(matrix_size, matrix_size, 1);
        matrix::Matrix matrix_dest            (matrix_size, matrix_size);

        size_t tests_in_bucket = static_cast<size_t>((16.0 / matrix_size) + 1);
        benchmarking::ResultT res = benchmarking::TestLatency([&]() {
            mult_func(matrix1, matrix2, matrix_dest); 
        }, 10, 8, tests_in_bucket);

        tests.push_back(Point(matrix_size, res));

        auto end_s   = std::chrono::steady_clock::now();
        test_time_s = end_s - start_s;

        assets::config::DoStep(matrix_size, assets::config::SQUARE_MATRICIES_SIZE_MUL_STEP);
        // matrix_size = static_cast<size_t>(static_cast<float>(matrix_size) * 1.25 + 0.5);

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

} // namespace dumb_math::matrix::tests::performance

using namespace dumb_math::matrix::tests::performance;

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

TEST(MatrixPerformance, BlockMulAvx256)
{
    RunMatrixBenchmark("BlockMulAvx256", dumb_math::matrix::Matrix::BlockMulAvx256_);
}
