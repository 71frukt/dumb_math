#include <concepts>
#include <cmath>

#include "mpreal.h"

namespace dumb_math {
namespace detail    {

// mpreal (256+ бит)
template <std::floating_point TestT>
[[nodiscard]] TestT CalcUlpError(const TestT testing, const mpfr::mpreal& standard_mpfr) 
{
    mpfr::mpreal::set_default_prec(256);

    if (std::isnan(testing) || mpfr::isnan(standard_mpfr)) 
    {
        return std::numeric_limits<TestT>::infinity();
    }

    if (std::isinf(testing) || mpfr::isinf(standard_mpfr)) 
    {
        if (std::isinf(testing) && mpfr::isinf(standard_mpfr) && (testing > 0) == (standard_mpfr > 0)) 
        {
            return TestT(0.0);
        }

        else
        {
            return std::numeric_limits<TestT>::infinity();
        }
    }

    if (mpfr::iszero(standard_mpfr)) 
    {
        mpfr::mpreal testing_mpfr = mpfr::abs(testing);
        mpfr::mpreal denorm_min   = std::numeric_limits<TestT>::denorm_min();
        return static_cast<TestT>(testing_mpfr / denorm_min);
    }

    const mpfr::mpreal standard_mpfr_abs = mpfr::abs(standard_mpfr);

    // mpfr::mpreal::get_exp() возвращает экспоненту в формате где мантисса лежит в [0.5, 1), а не [1, 2)
    // поэтому для совместимости с нашей математикой нужно вычесть 1.
    long exp = standard_mpfr_abs.get_exp() - 1;
    constexpr int p = std::numeric_limits<TestT>::digits - 1;

    mpfr::mpreal ulp_weight = mpfr::pow(2.0, exp - p);

    mpfr::mpreal testing_mpfr = testing;

    mpfr::mpreal diff = mpfr::abs(testing_mpfr - standard_mpfr);
    mpfr::mpreal error_in_ulps = diff / ulp_weight;

    return static_cast<TestT>(error_in_ulps);
}

} // namespace detail
} // namespace dumb_math
