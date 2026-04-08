#include <cstddef>
#include <iostream>
#include <memory>
#include <fstream>
#include <string_view>

namespace dumb_math::logarithm::detail::lookup_table {

struct AppSettings
{
public:
    bool parce(const int argc, const char *argv[]);

    std::ostream* ostream = nullptr;
    size_t table_size_exp = 8;     // table_size = 2^k, k := table_size_exp

private:
    std::unique_ptr<std::ofstream> file_output_storage_;
    
    static constexpr std::string_view DefaultOutputFileName_ = "source/dumb_math/modules/logarithm/generated/lookup_table.hpp";
};

} // namespace dumb_math::logarithm::detail::lookup_table
