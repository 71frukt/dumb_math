#include <cmath>
#include <concepts>
#include <cstddef>
#include <stdexcept>
#include <vector>

#include "RLogSU/logger.hpp"

#include "lookup_table_builder/lookup_table_builder.hpp"
#include "logarithm/common/approx.hpp"

namespace dumb_math::logarithm {
namespace detail::lookup_table {

[[nodiscard]] std::vector<TableItem> BuildTable(const size_t table_size_exp)
{
    if (table_size_exp > 63)
    {
        RLSU_THROW<std::runtime_error>("too large table_size_exp, interval_num overflow!");
    }

    size_t intervals_num = size_t(1) << table_size_exp;
    const size_t table_size = intervals_num + 1;

    std::vector<TableItem> table(table_size);

    long double delta = 1 / (long double) intervals_num;
    long double x_i   = 1;
    // long double x_i   = 1 + delta / 2;

    for (size_t i = 0; i < table_size; ++i, x_i += delta)
    {
        table[i].x         = x_i;
        table[i].one_div_x = 1 / x_i;
        table[i].ln_x      = common::LnArtgTailorBestAcc_0to2(x_i);
    }

    // ln(x_i = 2)
    table[table_size - 1].ln_x = 0.693147180559945309417232121458176568L;

    return table;
}

} // namespace detail::lookup_table    
} // namespace dumb_math::logarithm