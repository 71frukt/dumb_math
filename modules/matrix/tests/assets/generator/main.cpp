#include <cstddef>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <limits>
#include <stdexcept>

#include "RLogSU/logger.hpp"
#include "assets/config.hpp"
#include "common/matrix_iostream.hpp"
#include "matrix/matrix.hpp"
#include "rnd_bit_generators/mrg32k3a.hpp"

namespace dumb_math::matrix::tests::assets::generator {

static void FillMatrixRandom(Matrix& matrix)
{
    auto generator = random::MRG32k3a();
    float max_float = std::numeric_limits<float>::max();

    for (size_t row = 0; row < matrix.rows; ++row)
    {
        for (size_t col = 0; col < matrix.cols; ++col)
        {
            matrix[row, col] = generator.generate<float>(-max_float, max_float);
        }
    }
}

static void MakeRandomMatrixBlock(size_t rows, size_t cols, size_t num_in_block)
{
    using namespace iostream;

    std::filesystem::path matr_dir = config::GetMatrixFolder(cols, rows);
    std::filesystem::create_directories(matr_dir);

    for (size_t i = 0; i < num_in_block; ++i)
    {
        Matrix random_matrix(rows, cols);
        FillMatrixRandom(random_matrix);

        std::filesystem::path matrix_file_path = config::GetMatrixDir(cols, rows, i);
        std::ofstream matrix_file(matrix_file_path);

        if (!matrix_file)
            RLSU_THROW<std::runtime_error>("error occured while opening matrix_file");

        matrix_file << random_matrix;
    }
}

static void GenerateRectangleMatricies()
{
    for (size_t rows = 2; rows <= config::RECTANGLE_MATRICIES_MAX_ROWS; ++rows)
    {
        for (size_t cols = 2; cols <= config::RECTANGLE_MATRICIES_MAX_COLS; ++cols)
        {
            if (cols == rows)
                continue;

            MakeRandomMatrixBlock(rows, cols, config::RECTANGLE_MATRICIES_BLOCK_SIZE);
        }
    }
}

static void GenerateSquareMatricies()
{
    for (size_t n = 2; n <= config::SQUARE_MATRICIES_MAX_SIZE; ++n)
    {
        MakeRandomMatrixBlock(n, n, config::SQUARE_MATRICIES_BLOCK_SIZE);
    }
}

int main()
{
    GenerateSquareMatricies();
    GenerateRectangleMatricies();

    return 0;
}

} // namespace dumb_math::matrix::tests::assets::generator