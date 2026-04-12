#include <cstddef>
#include <vector>
#include <algorithm>

#include "matrix/matrix.hpp"
#include "RLogSU/logger.hpp"

namespace dumb_math::matrix {

bool Matrix::operator==(const Matrix& other) const noexcept
{
    if (this->rows != other.rows || this->cols != other.cols)
    {
        return false;
    }

    for (size_t row = 0; row < this->rows; row++)
    {
        for (size_t col = 0; col < this->cols; col++)
        {
            if ((*this)[row, col] != other[row, col])
            {
                return false;
            }
        }
    }

    return true;
}

float& Matrix::at(size_t i, size_t j)
{
    return data_.at(i * cols + j);
}

float Matrix::at(size_t i, size_t j) const
{
    return data_.at(i * cols + j);
}


void Matrix::AssertMatixMulConsistency_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{
    RLSU_ASSERT(matrix1.cols == matrix2.rows &&
                matrix1.rows == matrix_dest.rows && matrix2.cols == matrix_dest.cols,
                "inconsistent matrices: matrix1 [{}x{}], matrix2[{}x{}], matrix_dest[{}x{}]", 
                                        matrix1.rows,     matrix1.cols, 
                                        matrix2.rows,     matrix2.cols,
                                        matrix_dest.rows, matrix_dest.cols);
}


static float LineMulCol(const Matrix& matrix1, const Matrix& matrix2, const size_t line, const size_t col)
{
    RLSU_ASSERT(matrix1.rows >= line && matrix2.cols >= col);
    RLSU_ASSERT(matrix1.cols == matrix2.rows, "matrix1.cols = {}, matrix2.rows = {}", matrix1.cols, matrix2.rows);

    float matrix_dest = 0;

    for (size_t i = 0; i < matrix1.cols; i++)
    {
        matrix_dest += matrix1[line, i] * matrix2[i, col];
    }

    return matrix_dest;
}

void Matrix::DumbMul0_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{
    AssertMatixMulConsistency_(matrix1, matrix2, matrix_dest);
    
    for (size_t row = 0; row < matrix_dest.rows; row++)
    {
        for (size_t col = 0; col < matrix_dest.cols; col++)
        {
            matrix_dest[row, col] = LineMulCol(matrix1, matrix2, row, col);
        }
    }
}


void Matrix::DumbMul1_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{
    AssertMatixMulConsistency_(matrix1, matrix2, matrix_dest);

    for (size_t row = 0; row < matrix1.rows; row++)
    {
        for (size_t i = 0; i < matrix1.cols; i++)
        {
            float matrix1_val = matrix1[row, i];
            
            for (size_t col = 0; col < matrix2.cols; col++)
            {
                matrix_dest[row, col] += matrix1_val * matrix2[i, col];
            }
        }
    }
}



// (const Matrix* __restrict__ matrix1,  const Matrix* __restrict__ matrix2, Matrix* __restrict__ matrix_dest) 

void Matrix::OptMul0_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{

    const float* const m1_data = matrix1 .data_.data();
    const float* const m2_data = matrix2 .data_.data();
          float* const dest_data     = matrix_dest.data_.data();

    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(m1_data )  % 64) == 0 &&     "matrix1 is NOT 64-byte aligned!");
    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(m2_data )  % 64) == 0 &&     "matrix2 is NOT 64-byte aligned!");
    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(dest_data) % 64) == 0 && "matrix_dest is NOT 64-byte aligned!");

    AssertMatixMulConsistency_(matrix1, matrix2, matrix_dest);

    RLSU_ASSERT(m1_data != dest_data && m2_data != dest_data, "dest matrix is one of the source!");

    const float* __restrict__ m1_carr   = static_cast<const float*>(__builtin_assume_aligned(m1_data,   64));
    const float* __restrict__ m2_carr   = static_cast<const float*>(__builtin_assume_aligned(m2_data,   64));
          float* __restrict__ dest_carr = static_cast<      float*>(__builtin_assume_aligned(dest_data, 64));

    for (size_t row = 0; row < matrix1.rows; row++)
    {
        for (size_t i = 0; i < matrix1.cols; i++)
        {
            float matrix1_val = m1_carr[row * matrix1.cols + i];
    
            for (size_t col = 0; col < matrix2.cols; col++)
            {
                dest_carr[row * matrix_dest.cols + col] += matrix1_val * m2_carr[i * matrix2.cols + col];
            }
        }

    }

}


void Matrix::OptMul1_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{
    const float* const m1_data   = matrix1    .data_.data();
    const float* const m2_data   = matrix2    .data_.data();
          float* const dest_data = matrix_dest.data_.data();

    RLSU_ASSERT(m1_data != dest_data && m2_data != dest_data, "dest matrix is one of the source!");
    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(m1_data  ) % 64) == 0 &&     "matrix1 is NOT 64-byte aligned!");
    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(m2_data  ) % 64) == 0 &&     "matrix2 is NOT 64-byte aligned!");
    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(dest_data) % 64) == 0 && "matrix_dest is NOT 64-byte aligned!");
    AssertMatixMulConsistency_(matrix1, matrix2, matrix_dest);


    const float* __restrict__ m1_carr   = static_cast<const float*>(__builtin_assume_aligned(m1_data,   64));
    const float* __restrict__ m2_carr   = static_cast<const float*>(__builtin_assume_aligned(m2_data,   64));
          float* __restrict__ dest_carr = static_cast<      float*>(__builtin_assume_aligned(dest_data, 64));

    const size_t m1_rows   = matrix1.rows;
    const size_t m1_cols   = matrix1.cols;
    const size_t m2_cols   = matrix2.cols;
    const size_t dest_cols = matrix_dest.cols;

    for (size_t row = 0; row < m1_rows; row++)
    {
        for (size_t i = 0; i < m1_cols; i++)
        {
            float matrix1_val = m1_carr[row * m1_cols + i];
            
            for (size_t col = 0; col < m2_cols; col++)
            {
                dest_carr[row * dest_cols + col] += matrix1_val * m2_carr[i * m2_cols + col];
            }
        }
    }
}


void Matrix::BlockMul0_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{
    const float* const m1_data = matrix1.data_.data();
    const float* const m2_data = matrix2.data_.data();
    float* dest_data = matrix_dest.data_.data();

    RLSU_ASSERT(m1_data != dest_data && m2_data != dest_data, "dest matrix is one of the source!");
    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(m1_data  ) % 64) == 0 &&     "matrix1 is NOT 64-byte aligned!");
    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(m2_data  ) % 64) == 0 &&     "matrix2 is NOT 64-byte aligned!");
    RLSU_ASSERT((reinterpret_cast<std::uintptr_t>(dest_data) % 64) == 0 && "matrix_dest is NOT 64-byte aligned!");
    AssertMatixMulConsistency_(matrix1, matrix2, matrix_dest);

    const float* __restrict__ m1_carr   = static_cast<const float*>(__builtin_assume_aligned(m1_data,   64));
    const float* __restrict__ m2_carr   = static_cast<const float*>(__builtin_assume_aligned(m2_data,   64));
          float* __restrict__ dest_carr = static_cast<      float*>(__builtin_assume_aligned(dest_data, 64));

    const size_t m1_rows   = matrix1    .rows;
    const size_t m1_cols   = matrix1    .cols;
    const size_t m2_cols   = matrix2    .cols;
    const size_t dest_cols = matrix_dest.cols;

    const size_t block_size = 64;

    const size_t m1_rows_clean = (m1_rows / block_size) * block_size;
    const size_t m1_cols_clean = (m1_cols / block_size) * block_size;
    const size_t m2_cols_clean = (m2_cols / block_size) * block_size;

    for (size_t row_blk = 0; row_blk < m1_rows_clean; row_blk += block_size)
    {
        for (size_t i_blk = 0; i_blk < m1_cols_clean; i_blk += block_size)
        {
            for (size_t col_blk = 0; col_blk < m2_cols_clean; col_blk += block_size)
            {
                for (size_t row = row_blk; row < row_blk + block_size; ++row)
                {
                    const size_t dest_row_offset = row * dest_cols;
                    const size_t m1_row_offset   = row * m1_cols;

                    for (size_t i = i_blk; i < i_blk + block_size; ++i)
                    {
                        const float matrix1_val = m1_carr[m1_row_offset + i];
                        const size_t m2_row_offset = i * m2_cols;

                        for (size_t col = col_blk; col < col_blk + block_size; ++col)
                        {
                            dest_carr[dest_row_offset + col] += matrix1_val * m2_carr[m2_row_offset + col];
                        }
                    }
                }
            }
        }
    }
}


} // namespace dumb_math::matrix