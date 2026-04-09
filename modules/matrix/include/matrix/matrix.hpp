#pragma once

#include <cstddef>
#include <stddef.h>
#include <vector>

namespace dumb_math::matrix {

class Matrix
{
public:
    Matrix(size_t m, size_t n)
        : M(m)
        , N(n)
        , data_(m, std::vector<float>(n))
    {}

public:
    const std::vector<float>& operator[] (const size_t i) const noexcept;
          std::vector<float>& operator[] (const size_t i)       noexcept;
    const std::vector<float>& at         (const size_t i) const;
          std::vector<float>& at         (const size_t i);


    const size_t M;
    const size_t N;

private:

    std::vector<std::vector<float>> data_;

private:
    Matrix DumbMul0_(const Matrix& other) const;
    Matrix DumbMul1_(const Matrix& other) const;
};

} // namespace dumb_math::matrix