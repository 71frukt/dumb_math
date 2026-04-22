#include "rng_parallelizer/detail/cores_info_getter.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unistd.h>

namespace dumb_math::random {
namespace detail {

unsigned long GetCoreMaxFreq(int core_id)
{
    std::string path = "/sys/devices/system/cpu/cpu" + std::to_string(core_id) + "/cpufreq/cpuinfo_max_freq";
    std::ifstream file(path);
    unsigned long freq = 0;
    
    // Если файл недоступен (например, отключен драйвер cpufreq), вернется 0
    if (file >> freq)
        return freq;

    return 0; 
}

std::vector<int> GetPhysicalPCores()
{
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    
    if (num_cores <= 0)
        return {0};

    std::set<int> unique_physical_cores;
    std::vector<CoreInfo> core_stats;

    // Этап 1: Исключение Hyper-Threading (SMT)
    for (int i = 0; i < num_cores; ++i)
    {
        std::string path = "/sys/devices/system/cpu/cpu" + std::to_string(i) + "/topology/thread_siblings_list";
        std::ifstream file(path);
        
        if (file.is_open())
        {
            std::string line;
            std::getline(file, line);
            
            size_t pos = line.find_first_of(",-");
            int primary_sibling = std::stoi(pos == std::string::npos ? line : line.substr(0, pos));
            
            // Если физическое ядро встречается впервые, считываем его частоту
            if (unique_physical_cores.insert(primary_sibling).second)
            {
                unsigned long freq = GetCoreMaxFreq(primary_sibling);
                core_stats.push_back({primary_sibling, freq});
            }
        }
    }

    // Этап 2: Поиск пиковой частоты процессора (характеристика P-ядер)
    unsigned long max_system_freq = 0;

    for (const auto& core : core_stats)
    {
        if (core.max_freq > max_system_freq)
            max_system_freq = core.max_freq;
    }

    // Этап 3: Фильтрация P-cores
    std::vector<int> p_cores;
    // Допуск в 5% (0.95) на случай незначительных отклонений в репорте драйвера
    unsigned long threshold = max_system_freq * 0.95; 

    for (const auto& core : core_stats)
    {
        if (core.max_freq >= threshold)
            p_cores.push_back(core.logical_id);
    }

    if (p_cores.empty())
        p_cores.push_back(0);

    return p_cores;
}

} // namespace detail
} // namespace dumb_math::random