#include <iostream>
#include "matrix/matrix.hpp"

namespace dumb_math::matrix::tests::iostream {

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    for (size_t row = 0; row < matrix.rows; ++row)
    {
        for (size_t col = 0; col < matrix.cols; ++col)
        {
            os << matrix[row, col] << " ";
        }

        os << std::endl;
    }

    return os;
}

std::istream& operator>>(std::istream& is, Matrix& matrix)
{
    Matrix tmp = matrix;

    for (size_t row = 0; row < matrix.rows; ++row)
    {
        for (size_t col = 0; col < matrix.cols; ++col)
        {
            if (!(is >> tmp[row, col]))
                return is;                
        }
    }

    return is;
}

} // namespace dumb_math::matrix::tests::iostream