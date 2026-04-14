#pragma once

#include <iostream>
#include "matrix/matrix.hpp"

namespace dumb_math::matrix::tests::iostream {

std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
std::istream& operator>>(std::istream& is,       Matrix& matrix);

} // namespace dumb_math::matrix::tests::iostream