#pragma once

#include <cstddef>
#include <cstdint>
#include <concepts>
#include <limits>
#include <cfenv>
#include <cerrno>

#include "RLogSU/logger.hpp"
#include "lookup_table.hpp"
#include "logarithm/tailor.hpp"
#include "logarithm/common/approx.hpp"


namespace dumb_math::logarithm {

template <std::floating_point T, size_t TailorDeg = (sizeof(T) / 2)>
T ln(T x)
{
    if (x != x) // is nan
    {
        return x; // Передаем NaN дальше без генерации новых исключений
    }

    if (x < T(0.0))
    {
        errno = EDOM;
        std::feraiseexcept(FE_INVALID);

        return std::numeric_limits<T>::quiet_NaN();
    }

    if (x == 0)
    {
        errno = ERANGE;
        std::feraiseexcept(FE_DIVBYZERO);

        return -std::numeric_limits<T>::infinity();
    }

    if (x == std::numeric_limits<T>::infinity()) 
    {
        return x; // ln(+inf) = +inf. Флаги не выставляются.
    }

    // if (x >= T(0.85) && x <= T(1.15))
    // {
    //     return common::LnArtgTailor_0to2<TailorDeg * 2>(x);
    // }

    constexpr static long double ln2 = 0.693147180559945309417232121458176568L;

    using namespace logarithm::detail::lookup_table;    

    const auto     mantissa    = common::GetNormalizedMantissa(x);
    const auto     exponenta   = common::GetExponent(x);
    const uint32_t table_index = GetTableIndex<TableSizeExp>(mantissa);

    RLSU_ASSERT(table_index < LookupTable.size(), "");

    T v = LookupTable[table_index].one_div_x * T(mantissa);

    T ln_x = exponenta * ln2 + LookupTable[table_index].ln_x + TailorLn<TailorDeg>(v);

    return ln_x;
}

} // namespace dumb_math::logarithm