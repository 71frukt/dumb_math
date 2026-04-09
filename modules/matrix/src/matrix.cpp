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

Matrix Matrix::DumbMul_(const Matrix& other) const
{
    if (!(this->M == this->N == other.M == other.N))
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("inconsistent matrices: this->M = {}, this->N = {}, other.M = {}, other.N = {}", 
                                                                                        this->M,      this->N,      other.M,      other.N));
    
    


    return Matrix(1, 1);
}


} // namespace dumb_math::matrix