#pragma once

#include <cstddef>
#include <stddef.h>
#include <vector>

#include "detail/matrix_data_allocator.hpp"

namespace dumb_math::matrix {

class Matrix
{
public:
    Matrix(size_t rows, size_t cols)
        : rows_(rows)
        , cols_(cols)
        , stride_((cols + 15) & ~static_cast<size_t>(15)) 
        , data_(rows_ * stride_)
    {}

public:
    bool operator==(const Matrix& other) const noexcept;

    __attribute__((always_inline)) inline float& operator[](size_t i, size_t j) noexcept
    {
        return data_[i * stride_ + j];
    }

    __attribute__((always_inline)) inline float operator[](size_t i, size_t j) const noexcept
    {
        return data_[i * stride_ + j];
    }

    float& at(size_t i, size_t j);
    float  at(size_t i, size_t j) const;

public:
    size_t rows  () const noexcept { return rows_; }
    size_t cols  () const noexcept { return cols_; }
    size_t stride() const noexcept { return stride_; }

private:
    size_t rows_;
    size_t cols_;
    size_t stride_;

    // std::vector<float> data_;
    std::vector<float, detail::AlignedAllocator<float, 64>> data_;

    static void AssertMatixMulConsistency_(const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);

// private:
public:
    static void DumbMul0_       (const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);
    static void DumbMul1_       (const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);
    static void OptMul0_        (const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);
    static void OptMul1_        (const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);
    static void BlockMul0_      (const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);
    static void BlockMulAvx256_ (const Matrix& matrix1, const Matrix& matrix2, Matrix& matrix_dest);
};

} // namespace dumb_math::matrix