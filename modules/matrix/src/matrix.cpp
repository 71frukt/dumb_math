#include <cstddef>
#include <stdexcept>
#include <vector>

#include "matrix/matrix.hpp"
#include "RLogSU/logger.hpp"

namespace dumb_math::matrix {

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


static float LineMulCol(const Matrix& m1, const Matrix& m2, const size_t line, const size_t col)
{
    RLSU_ASSERT(m1.M >= line && m2.N >= col);
    RLSU_ASSERT(m1.N == m2.M, "m1.N = {}, m2.M = {}", m1.N, m2.M);

    float result = 0;

    for (size_t i = 0; i < m1.N; i++)
    {
        result += m1[line][i] * m2[i][col];
    }

    return result;
}

Matrix Matrix::DumbMul0_(const Matrix& other) const
{
    if (!(this->N == other.M))
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("inconsistent matrices: this->M = {}, this->N = {}, other.M = {}, other.N = {}", 
                                                                                        this->M,      this->N,      other.M,      other.N));
    Matrix result(this->M, other.N);

    for (size_t m = 0; m < result.M; m++)
    {
        for (size_t n = 0; n < result.N; n++)
        {
            result[m][n] = LineMulCol(*this, other, m, n);
        }
    }

    return result;
}


Matrix Matrix::DumbMul1_(const Matrix& other) const
{
    if (this->N != other.M)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("inconsistent matrices: this->M = {}, this->N = {}, other.M = {}, other.N = {}", 
                                                                                        this->M,      this->N,      other.M,      other.N));
    
    Matrix result(this->M, other.N);

    for (size_t m = 0; m < this->M; m++)
    {
        for (size_t i = 0; i < this->N; i++)
        {
            float m1_val = (*this)[m][i];
            
            for (size_t n = 0; n < other.N; n++)
            {
                result[m][n] += m1_val * other[i][n];
            }
        }
    }

    return result;
}


} // namespace dumb_math::matrix