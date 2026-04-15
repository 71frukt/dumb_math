#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>


#include "RLogSU/logger.hpp"
#include "assets/config.hpp"
#include "matrix/matrix.hpp"
#include "e2e_mul.hpp"

#ifndef TASKS_DIR
#define TASKS_DIR "."
#endif

#ifndef KEYS_DIR
#define KEYS_DIR "."
#endif

#ifndef CUR_SOURCE_DIR
#define CUR_SOURCE_DIR "."
#endif

constexpr double E2E_TESTS_MATRIX_SIZE_STEP = dumb_math::matrix::tests::assets::config::RECTANGLE_MATRICIES_SIZE_MUL_STEP;

namespace fs = std::filesystem;

namespace {

static void MakeTask(fs::path task_path, fs::path m1_path, fs::path m2_path)
{
    fs::path m1_rel_path = m1_path.lexically_relative(CUR_SOURCE_DIR);
    fs::path m2_rel_path = m2_path.lexically_relative(CUR_SOURCE_DIR);
    
    std::ofstream task_file(task_path);
    if (!task_file)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to open '{}'", task_path.string()));

    task_file << m1_rel_path.string() << std::endl;  
    task_file << m2_rel_path.string() << std::endl; 

    task_file.close();

    if (!task_file)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to write task in '{}'", task_path.string()));
}

}


int main() try
{
    using namespace dumb_math::matrix::tests;
    using namespace dumb_math::matrix::tests::correctness::e2e;
    
    size_t test_num = 0;
    const fs::path tasks_dir(TASKS_DIR);
    const fs::path keys_dir (KEYS_DIR );

    for (size_t row = 2; row < assets::config::RECTANGLE_MATRICIES_MAX_ROWS; assets::config::DoStep(row, E2E_TESTS_MATRIX_SIZE_STEP))
    {
        for (size_t col = 2; col < assets::config::RECTANGLE_MATRICIES_MAX_COLS; assets::config::DoStep(col, E2E_TESTS_MATRIX_SIZE_STEP))
        {
            fs::path task_path = tasks_dir / (std::to_string(test_num) + ".txt");
            fs::path key_path  = keys_dir  / (std::to_string(test_num) + ".txt");
            
            auto matr1_info = assets::config::RequestMatrix(row,             row / 2, 0);
            auto matr2_info = assets::config::RequestMatrix(matr1_info.cols, col,     0);

            if (matr1_info.cols != matr2_info.rows)
                continue;

            // RLSU_INFO("[{}x{}], [{}x{}]\n", matr1_info.rows, matr1_info.cols, matr2_info.rows, matr2_info.cols);

            ::MakeTask(task_path, matr1_info.filepath, matr2_info.filepath);
            MatrixE2eMul(dumb_math::matrix::Matrix::DumbMul1_, matr1_info, matr2_info, key_path);
            
            ++test_num;
        }
    }
}
catch (std::runtime_error e)
{
    RLSU_LOG_RUNTIME_ERR(e);
}

catch (std::exception e)
{
    std::cout << "EXCEPTION: " << e.what() << std::endl;
}