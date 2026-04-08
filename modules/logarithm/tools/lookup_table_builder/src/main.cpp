#include <exception>
#include <iostream>
#include <stdexcept>

#include "RLogSU/logger.hpp"
#include "cmd_options/cmd_options.hpp"
#include "lookup_table_builder/lookup_table_builder.hpp"

int main(const int argc, const char* argv[]) try
{
    using namespace dumb_math::logarithm::detail::lookup_table;
    
    AppSettings settings;
    settings.parce(argc, argv);

    std::vector<TableItem> generated_table = BuildTable(settings.table_size_exp);
    PrintTable(generated_table, settings.table_size_exp, *settings.ostream);

    return 0;
}
catch (std::runtime_error e)
{
    RLSU_LOG_RUNTIME_ERR(e);
}
catch (std::exception e)
{
    std::cout << e.what();
}