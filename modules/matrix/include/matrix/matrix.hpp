#pragma once

#include <cstddef>
#include <stddef.h>
#include <vector>

#include "matrix/detail/matrix_data_allocator.h"

namespace dumb_math::matrix {

class Matrix
{
public:
    Matrix(size_t m, size_t n)
        : rows(m)
        , cols(n)
        , data_(m * n)
    {}

public:
    bool operator==(const Matrix& other) const noexcept;

    __attribute__((always_inline)) inline float& operator[](size_t i, size_t j) noexcept
    {
        return data_[i * cols + j];
    }

    __attribute__((always_inline)) inline float operator[](size_t i, size_t j) const noexcept
    {
        return data_[i * cols + j];
    }

    float& at(size_t i, size_t j);
    float  at(size_t i, size_t j) const;

    const size_t rows;
    const size_t cols;

private:

    std::vector<float, detail::AlignedAllocator<float, 64>> data_;

    static void AssertMatixMulConsistency_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);

// private:
public:
    static void DumbMul0_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);
    static void DumbMul1_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);
    static void OptMul0_ (const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);

};

} // namespace dumb_math::matrix