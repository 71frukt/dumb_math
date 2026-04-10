#include <cstddef>
#include <stdexcept>
#include <vector>

#include "matrix/matrix.hpp"
#include "RLogSU/logger.hpp"

namespace dumb_math::matrix {

bool Matrix::operator==(const Matrix& other) const noexcept
{
    if (this->M != other.M || this->N != other.N)
    {
        return false;
    }

    for (size_t m = 0; m < this->M; m++)
    {
        if ((*this)[m] != other[m])
        {
            return false;
        }
    }

    return true;
}

std::vector<float>& Matrix::operator[](const size_t i) noexcept
{
    return data_[i];
}

const std::vector<float>& Matrix::operator[](const size_t i) const noexcept
{
    return data_[i];
}

std::vector<float>& Matrix::at(const size_t i)
{
    return data_.at(i);
}

const std::vector<float>& Matrix::at(const size_t i) const
{
    return data_.at(i);
}

void Matrix::AssertMatixMulConsistency_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{
    RLSU_ASSERT(matrix1.N == matrix2.M &&
                matrix1.M == matrix_dest.M && matrix2.N == matrix_dest.N,
                "inconsistent matrices: matrix1 [{}x{}], matrix2[{}x{}], matrix_dest[{}x{}]", 
                                        matrix1.M,     matrix1.N, 
                                        matrix2.M,     matrix2.N,
                                        matrix_dest.M, matrix_dest.N);
}


static float LineMulCol(const Matrix& matrix1, const Matrix& matrix2, const size_t line, const size_t col)
{
    RLSU_ASSERT(matrix1.M >= line && matrix2.N >= col);
    RLSU_ASSERT(matrix1.N == matrix2.M, "matrix1.N = {}, matrix2.M = {}", matrix1.N, matrix2.M);

    float matrix_dest = 0;

    for (size_t i = 0; i < matrix1.N; i++)
    {
        matrix_dest += matrix1[line][i] * matrix2[i][col];
    }

    return matrix_dest;
}

void Matrix::DumbMul0_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{
    AssertMatixMulConsistency_(matrix1, matrix2, matrix_dest);
    
    for (size_t m = 0; m < matrix_dest.M; m++)
    {
        for (size_t n = 0; n < matrix_dest.N; n++)
        {
            matrix_dest[m][n] = LineMulCol(matrix1, matrix2, m, n);
        }
    }
}


void Matrix::DumbMul1_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest)
{
    AssertMatixMulConsistency_(matrix1, matrix2, matrix_dest);

    for (size_t m = 0; m < matrix1.M; m++)
    {
        for (size_t i = 0; i < matrix1.N; i++)
        {
            float matrix1_val = matrix1[m][i];
            
            for (size_t n = 0; n < matrix2.N; n++)
            {
                matrix_dest[m][n] += matrix1_val * matrix2[i][n];
            }
        }
    }
}


} // namespace dumb_math::matrix