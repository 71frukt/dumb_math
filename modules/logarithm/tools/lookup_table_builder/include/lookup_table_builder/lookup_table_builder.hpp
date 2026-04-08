#pragma once

#include <iostream>
#include <ostream>
#include <vector>

#include "logarithm/lookup_table_item.hpp"

namespace dumb_math::logarithm {
namespace detail::lookup_table {

#define LN_LOOKUP_TABLE  LookupTable

[[nodiscard]] std::vector<TableItem> BuildTable(const size_t table_size_exp);

void PrintTable(const std::vector<TableItem>& table, const size_t table_size_exp, std::ostream& ostream);

} // namespace detail::lookup_table 
} // namespace dumb_math::logarithm 