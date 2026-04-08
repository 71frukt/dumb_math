#pragma once

#include "mpreal.h"
#include <concepts>

#include "logarithm/logarithm.hpp"
#include "dumb_math/detail/calc_ulp_error.hpp"

namespace dumb_math        {
namespace logarithm::tests {

template <std::floating_point TestT>
TestT CalcLnUlpErr(TestT x)
{
    mpfr::mpreal x_mpfr = x;

    mpfr::mpreal expected_ln = mpfr::log(x_mpfr);
    TestT        my_ln       = dumb_math::logarithm::ln(x);

    TestT ulp_error = dumb_math::detail::CalcUlpError(my_ln, expected_ln);

    return ulp_error;
}

} // namespace logarithm::tests
} // namespace dumb_math