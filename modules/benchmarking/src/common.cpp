#include "benchmarking/common.hpp"

#include "RLogSU/logger.hpp"
#include <functional>
#include <fstream>

namespace dumb_math::benchmarking {

namespace detail {
    
void CheckCpuGovernor()
{
    std::ifstream governor_file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
    std::string governor;

    if (governor_file >> governor)
    {
        if (governor != "performance")
        {
            RLSU_WARNING("CPU governor is set to '{}''. Frequency scaling is enabled.\n"
                                "Benchmark results may be noisy.                           \n"
                                "Run `sudo ./utils/set_performance.sh` before testing.      \n");
        }
    }
}

} // namespace detail


void ExportResultsToCSV(std::string curve_name, const std::vector<std::pair<double, ResultT>>& data, const std::string& filename) 
{
    std::ofstream file(filename);

    file << curve_name << std::endl;
    file << "x, average, standard_deviation" << std::endl;
    
    for (const auto& [n, result] : data) 
    {
        file << n << "," << result.average << "," << result.standard_deviation << "\n";
    }
}

} // namespace dumb_math::performante_tests