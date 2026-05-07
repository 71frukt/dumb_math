#include <cstddef>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "RLogSU/logger.hpp"
#include "assets/config.hpp"
#include "common/matrix_iostream.hpp"
#include "matrix/matrix.hpp"
#include "engines/mrg32k3a/generator.hpp"

namespace dumb_math::matrix::tests::assets::generator {

static void FillMatrixRandom(Matrix& matrix)
{
    static auto generator = random::MRG32k3a();
    float max_float = 2000;

    for (size_t row = 0; row < matrix.rows(); ++row)
    {
        for (size_t col = 0; col < matrix.cols(); ++col)
        {
            matrix[row, col] = generator.generate<float>(-max_float, max_float);
        }
    }
}

static void MakeRandomMatrixBlock(size_t rows, size_t cols, size_t block_size)
{
    using namespace tests::iostream;

    struct PairHash {
        std::size_t operator()(const std::pair<size_t, size_t>& p) const noexcept {
            std::size_t h1 = std::hash<size_t>{}(p.first);
            std::size_t h2 = std::hash<size_t>{}(p.second);
            
            // Классический алгоритм смешивания хэшей (аналог boost::hash_combine)
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2)); 
        }
    };

    size_t cur_block_size = 0;

    static std::unordered_map<std::pair<size_t, size_t>, size_t, PairHash> blocks_sizes;
    if (blocks_sizes.contains(std::pair(rows, cols)))
    {
        cur_block_size += blocks_sizes[std::pair(rows, cols)];
    }

    std::filesystem::path matr_dir = config::GetMatrixFolder(rows, cols);
    std::filesystem::create_directories(matr_dir);

    for (size_t i = 0; i < block_size; ++i)
    {
        Matrix random_matrix(rows, cols);
        FillMatrixRandom(random_matrix);

        std::filesystem::path matrix_file_path = config::GetMatrixDir(rows, cols, cur_block_size + i);
        std::ofstream matrix_file(matrix_file_path);

        if (!matrix_file)
            RLSU_THROW<std::runtime_error>("error occured while opening matrix_file");

        matrix_file << random_matrix;
    }
}

static void GenerateRectangleMatricies()
{
    for (size_t rows = 2; rows <= config::RECTANGLE_MATRICIES_MAX_ROWS; config::DoStep(rows, config::RECTANGLE_MATRICIES_SIZE_MUL_STEP))
    {
        for (size_t cols = 2; cols <= config::RECTANGLE_MATRICIES_MAX_COLS; config::DoStep(cols, config::RECTANGLE_MATRICIES_SIZE_MUL_STEP))
        {
            // если квадратная, положим в блок для квадратных
            MakeRandomMatrixBlock(rows, cols, config::RECTANGLE_MATRICIES_BLOCK_SIZE);
        }
    }
}

static void GenerateSquareMatricies()
{
    for (size_t n = 2; n <= config::SQUARE_MATRICIES_MAX_SIZE; config::DoStep(n, config::SQUARE_MATRICIES_SIZE_MUL_STEP))
    {
        MakeRandomMatrixBlock(n, n, config::SQUARE_MATRICIES_BLOCK_SIZE);
    }
}

} // namespace dumb_math::matrix::tests::assets::generator

int main()
{
    dumb_math::matrix::tests::assets::generator::GenerateSquareMatricies();
    dumb_math::matrix::tests::assets::generator::GenerateRectangleMatricies();

    return 0;
}
