#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <string>

#include "stat_generator.hpp"
#include "RLogSU/logger.hpp"
#include "statistics/generated/config.hpp"

const std::string ODataFolderName  = CURRENT_SRC_DIR "/data/";
const std::string OPlotsFolderName = CURRENT_SRC_DIR "/plots/";

namespace {
    
    using namespace dumb_math::logarithm::tests::statistics;

    template <typename T>
    void MakeStat(const std::string_view base_name_1toInf, const std::string_view base_name_near1)
    {
        std::string path_1toInf = std::string(config::DATA_DIR) + "/" + std::string(base_name_1toInf) + ".txt";
        std::string path_near1  = std::string(config::DATA_DIR) + "/" + std::string(base_name_near1)  + ".txt";

        std::ofstream stat_1toInf(path_1toInf);
        std::ofstream stat_near1 (path_near1);

        if (!stat_1toInf.is_open())
            RLSU_THROW<std::runtime_error>(RLSU_FORMAT("file '{}' is not open!", path_1toInf));

        if (!stat_near1.is_open())
            RLSU_THROW<std::runtime_error>(RLSU_FORMAT("file '{}' is not open!", path_near1));

        GenerateLnErrorUlpData_1toInf<T>(stat_1toInf);
        GenerateLnErrorUlpData_near1 <T>(stat_near1 );
    }
}


int main(const int argc, const char* argv[]) try
{
    #ifdef DUMBTEST_LOG_FLOAT
        ::MakeStat<float>       (config::FILE_1TOINF_FLOAT,   config::FILE_NEAR1_FLOAT );
    #endif

    #ifdef DUMBTEST_LOG_DOUBLE
        ::MakeStat<double>      (config::FILE_1TOINF_DOUBLE,  config::FILE_NEAR1_DOUBLE);
    #endif
    
    #ifdef DUMBTEST_LOG_LDOUBLE
        ::MakeStat<long double> (config::FILE_1TOINF_LDOUBLE, config::FILE_NEAR1_LDOUBLE);
    #endif
}

catch (std::runtime_error e)
{
    RLSU_LOG_RUNTIME_ERR(e);
}