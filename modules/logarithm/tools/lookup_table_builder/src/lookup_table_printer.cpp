#include <cmath>
#include <vector>
#include <limits>
#include <iomanip>

#include "RLogSU/logger.hpp"

#include "lookup_table_builder/lookup_table_builder.hpp"

namespace dumb_math::logarithm {
namespace detail::lookup_table {

void PrintTable(const std::vector<TableItem>& table, const size_t table_size_exp, std::ostream& ostream)
{
    RLSU_ASSERT(table.size() == (size_t(1) << table_size_exp) + 1, "invalid table size={} or table_size_exp={}", table.size(), table_size_exp);

    #define STR_HELPER_(x) #x
    #define STR_(x) STR_HELPER_(x)

    ostream << "// generated file"                                              << std::endl
            << "// do not change it manually"                                   << std::endl
            <<                                                                     std::endl
            << "#pragma once"                                                   << std::endl
            <<                                                                     std::endl            
            << "#include \"logarithm/lookup_table_item.hpp\""                   << std::endl
            <<                                                                     std::endl            
            << "#include <array>"                                               << std::endl
            << "#include <cstddef>"                                             << std::endl            
            <<                                                                     std::endl            
            << "namespace dumb_math::logarithm {"                               << std::endl
            << "namespace detail::lookup_table {"                               << std::endl
            <<                                                                     std::endl
            << "constexpr size_t TableSizeExp = " << table_size_exp << ";"      << std::endl
            <<                                                                     std::endl
            << "constexpr std::array<TableItem, " << table.size() << "> "
            << STR_(LN_LOOKUP_TABLE) << " = {{"                                 << std::endl;

    #undef STR_HELPER_
    #undef STR_

    ostream << std::fixed << std::setprecision(std::numeric_limits<long double>::max_digits10);

    for (const TableItem& item : table)
    {
        ostream << "    {.x = "         << item.x         << "L, "
                        ".one_div_x = " << item.one_div_x << "L, "
                        ".ln_x = "      << item.ln_x      << "L},"               << std::endl;
    }

    ostream << "}};"                                                            << std::endl
            <<                                                                     std::endl
            << ""
            << "} //namespace dumb_math::logarithm"                             << std::endl
            << "} //namespace detail::lookup_table"                             << std::endl;
}


} // namespace detail::lookup_table    
} // namespace dumb_math::logarithm