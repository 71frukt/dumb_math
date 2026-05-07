#pragma once

#include <vector>
#include <thread>

namespace dumb_math::random::tests {

inline std::vector<int> GetCores()
{
    unsigned available_threads = std::thread::hardware_concurrency();
    
    if (available_threads == 0)
        available_threads = 1;

    std::vector<int> cores;
    for (unsigned i = 0; i < available_threads; ++i)
    {
        cores.push_back(i);
    }

    return cores;
}

} // namespace dumb_math::random::tests
