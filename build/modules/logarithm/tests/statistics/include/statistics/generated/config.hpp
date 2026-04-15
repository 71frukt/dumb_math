// config.hpp.in
#pragma once
#include <string_view>

#define DUMBTEST_LOG_FLOAT
#define DUMBTEST_LOG_DOUBLE


namespace dumb_math          {
namespace logarithm::tests   {
namespace statistics::config {
    
constexpr std::string_view DATA_DIR = "/home/frukt71/Github/dumb_math_test/dumb_math/build/modules/logarithm/tests/statistics/data";

// имена файлов без расширения
constexpr std::string_view FILE_1TOINF_DOUBLE  = "stat_1toInf_double";
constexpr std::string_view FILE_NEAR1_DOUBLE   = "stat_near1_double";

constexpr std::string_view FILE_1TOINF_FLOAT   = "stat_1toInf_float";
constexpr std::string_view FILE_NEAR1_FLOAT    = "stat_near1_float";

constexpr std::string_view FILE_1TOINF_LDOUBLE = "";
constexpr std::string_view FILE_NEAR1_LDOUBLE  = "";

} // namespace statistics::config
} // namespace logarithm::tests
} // namespace dumb_math
