#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "benchmarking/latency_test.hpp" 
#include "matrix/matrix.hpp"

#ifndef DATA_DIR
#define DATA_DIR "."
#endif

template <typename Func>
void RunMatrixBenchmark(const std::string& test_name, Func mult_func) 
{
    using namespace dumb_math;

    matrix::Matrix matrix1    (128, 128);
    matrix::Matrix matrix2    (128, 128);
    matrix::Matrix matrix_dest(128, 128);

    benchmarking::ResultT res = benchmarking::TestLatency([&]() {
        mult_func(matrix1, matrix2, matrix_dest); 
    }, 10, 10, 10);

    namespace fs = std::filesystem;
    
    fs::path dir_path = fs::path(DATA_DIR) / test_name;
    fs::create_directories(dir_path);
    fs::path ofile_path = dir_path / "latency.txt";

    std::ofstream ofile(ofile_path);

    if (ofile.is_open()) 
    {
        double cv = res.standard_deviation / res.average;
        ofile << uint64_t(res.average) << " " << cv << "\n";
        ofile.close();
        
        std::cout << "[ BENCHMARK] " << test_name 
                  << " Latency: " << res.average 
                  << " cycles, CV: " << (cv * 100.0) << "%\n";
    }
    
    else
    {
        FAIL() << "Couldn't create or open a file: " << ofile_path;
    }
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