#pragma once

#include <vector>
#include <unistd.h>

namespace dumb_math::random {
namespace detail {

struct CoreInfo {
    int logical_id;
    unsigned long max_freq;
};

unsigned long GetCoreMaxFreq(int core_id);

std::vector<int> GetPhysicalPCores();

} // namespace detail
} // namespace dumb_math::random