// generated file

#pragma once
#include "RLogSU/logger.hpp"
#include <cstddef>
#include <string>
#include <filesystem>

namespace dumb_math      {
namespace matrix::tests  {
namespace assets::config {
    
const std::filesystem::path RECTANGLE_MATRICIES_DIR = "/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/assets/rectangle";
const std::filesystem::path SQUARE_MATRICIES_DIR    = "/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/assets/square";

// матрицы генерируются всевозможных размеров от 2 до максимального
// для каждого размера генерируется определённое количество матриц
constexpr size_t SQUARE_MATRICIES_MAX_SIZE         = 3000;
constexpr size_t SQUARE_MATRICIES_BLOCK_SIZE       = 2;

constexpr size_t RECTANGLE_MATRICIES_MAX_COLS      = 1024;
constexpr size_t RECTANGLE_MATRICIES_MAX_ROWS      = 1024;
constexpr size_t RECTANGLE_MATRICIES_BLOCK_SIZE    = 2;

// константа на которую домножается размер матрицы на каждом шаге цикла генерации 
constexpr double SQUARE_MATRICIES_SIZE_MUL_STEP    = 1.25;
constexpr double RECTANGLE_MATRICIES_SIZE_MUL_STEP = 1.7;

inline size_t DoStep(size_t& z, double mul_k)
{
    z = static_cast<size_t>(static_cast<double>(z) * mul_k + 0.5);
    return z;
}

inline std::filesystem::path GetMatrixFolder(const size_t rows, const size_t cols)
{
    const std::filesystem::path& base_dir = (cols == rows) ? SQUARE_MATRICIES_DIR : RECTANGLE_MATRICIES_DIR;

    std::string block_name = std::to_string(rows) + "x" + std::to_string(cols);

    return base_dir / block_name;
}

inline std::filesystem::path GetMatrixDir(const size_t rows, const size_t cols, const size_t num_in_block)
{
    std::filesystem::path folder = GetMatrixFolder(rows, cols);
    std::filesystem::path dir = folder / (std::to_string(num_in_block) + ".txt");
    
    return dir;
}

namespace detail {

inline constexpr size_t GetClosestGeneratedSize(size_t req_size, size_t max_size, double mul_step)
{
    if (req_size <= 2)
        return 2;

    size_t closest = 2;
    
    for (size_t val = 2; val <= max_size; DoStep(val, mul_step))
    {
        if (val > req_size) 
        {
            break;
        }

        closest = val;
    }
    
    return closest;
}

} // namespace detail

struct MatrixShopItem
{
    const size_t rows;
    const size_t cols;
    std::filesystem::path filepath;
};

inline MatrixShopItem RequestMatrix(const size_t req_rows, size_t req_cols, const size_t num_in_block)
{
    size_t cols = 2;
    size_t rows = 2;

    if (req_rows == req_cols)
    {
        rows = detail::GetClosestGeneratedSize(req_rows, SQUARE_MATRICIES_MAX_SIZE, SQUARE_MATRICIES_SIZE_MUL_STEP);
        cols = rows;
    }

    else
    {
        rows = detail::GetClosestGeneratedSize(req_rows, RECTANGLE_MATRICIES_MAX_ROWS, RECTANGLE_MATRICIES_SIZE_MUL_STEP);
        cols = detail::GetClosestGeneratedSize(req_cols, RECTANGLE_MATRICIES_MAX_COLS, RECTANGLE_MATRICIES_SIZE_MUL_STEP);
    }
    
    // Безопасный выбор номера матрицы в блоке через деление по модулю
    size_t actual_num = 0;

    if (cols == rows)
    {
        actual_num = num_in_block % SQUARE_MATRICIES_BLOCK_SIZE;

        if (num_in_block >= SQUARE_MATRICIES_BLOCK_SIZE)
            RLSU_WARNING("requested num_in_block={} > SQUARE_MATRICIES_BLOCK_SIZE={}", num_in_block, SQUARE_MATRICIES_BLOCK_SIZE);
    }
    
    else
    {
        actual_num = num_in_block % RECTANGLE_MATRICIES_BLOCK_SIZE;

        if (num_in_block >= RECTANGLE_MATRICIES_BLOCK_SIZE)
            RLSU_WARNING("requested num_in_block={} > RECTANGLE_MATRICIES_BLOCK_SI={}", num_in_block, RECTANGLE_MATRICIES_BLOCK_SIZE);
    }

    return {rows, cols,GetMatrixDir(rows, cols, actual_num)};
}

} // namespace assets::config
} // namespace matrix::tests
} // namespace dumb_math
