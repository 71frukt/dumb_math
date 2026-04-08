#include <cstdint>
#include <cstring>
#include <limits>
#include <bit>
#include <concepts>

#include "RLogSU/logger.hpp"
#include "dumb_math/common/get_floating_parts.hpp"

namespace dumb_math::logarithm {
namespace detail::lookup_table {

struct TableItem
{
    long double x;
    long double one_div_x;
    long double ln_x;    
};


template <size_t TableSizeExp, std::floating_point T>
uint32_t GetTableIndex(T normd_1to2_mantissa) 
{
    static_assert(TableSizeExp > 0 && TableSizeExp <= 32, "TableSizeExp is out of bounds");
    RLSU_ASSERT(normd_1to2_mantissa >= 1 && normd_1to2_mantissa <= 2, "normd_1to2_mantissa={} out of bounds", normd_1to2_mantissa);

    const auto fraction = common::GetMantissa(normd_1to2_mantissa);
    constexpr int frac_bits = std::numeric_limits<T>::digits - 1;

    static_assert(TableSizeExp <= static_cast<size_t>(frac_bits), 
                  "TableSizeExp exceeds available fraction bits");

    constexpr int shift_amount = frac_bits - TableSizeExp;
    constexpr uint64_t round_const = 1ULL << (shift_amount - 1);

    return static_cast<uint32_t>((fraction + round_const) >> shift_amount);
}

} // namespace dumb_math::logarithm
} // namespace detail::lookup_table