#include "cmd_options.hpp"
#include "cxxopts.hpp"
#include "RLogSU/logger.hpp"
#include <string>

namespace dumb_math::logarithm::detail::lookup_table {

bool AppSettings::parce(const int argc, const char *argv[])
{
    cxxopts::Options options("lookup_table_builder");

    options.add_options()
        ("o,output"        , "Path to output"                       , cxxopts::value<std::string>())
        ("k,table_size_exp", "table_size = 2^k, k := table_size_exp", cxxopts::value<std::size_t>())
        ("h,help"          , "SOS")
        ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return false;
    }

    //-------------------------output-----------------------------------------------------------

    std::string output_filename;

    if (result.count("output"))
        output_filename = result["output"].as<std::string>();       

    else
        output_filename = DefaultOutputFileName_;

    file_output_storage_ = std::make_unique<std::ofstream>(output_filename);

    if (!file_output_storage_->is_open())
    {
        throw std::runtime_error("Could not open file: '" + output_filename + "'");
    }
    
    ostream = file_output_storage_.get();


    //-------------------------table_size_exp-------------------------------------------------------------
    if (result.count("table_size_exp"))
    {
        table_size_exp = result["table_size_exp"].as<size_t>();
    }

    return true;
}

} // namespace dumb_math::logarithm::detail::lookup_table