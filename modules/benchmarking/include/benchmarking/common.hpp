#pragma once

#include "RLogSU/logger.hpp"
#include <cstdint>
#include <functional>
#include <fstream>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#include <cpuid.h>
#endif

namespace dumb_math::benchmarking {

struct ResultT
{
    double average;
    double standard_deviation;
};


namespace detail {
    
template <class T>
__attribute__((always_inline)) inline void DoNotOptimize(const T& value)
{
    asm volatile("" : : "r,m"(value) : "memory");
}

__attribute__((always_inline)) inline void ClobberMemory()
{
    asm volatile("" ::: "memory");
}

template <typename Func>
__attribute__((always_inline)) inline void RunAndProtect(Func&& f)
{
    if constexpr (std::is_void_v<std::invoke_result_t<Func>>)
    {
        f();
        ClobberMemory();
    }
    
    else
    {
        DoNotOptimize(f());
    }
}

__attribute__((always_inline)) inline uint64_t StartTimer() 
{
    ClobberMemory();
    _mm_lfence();
    ClobberMemory();
    uint64_t start = __rdtsc();
    ClobberMemory();
    return start;
}

__attribute__((always_inline)) inline uint64_t StopTimer() 
{
    unsigned int dummy;
    ClobberMemory();
    uint64_t end = __rdtscp(&dummy);
    ClobberMemory();
    return end;
}

inline void CheckCpuGovernor()
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

} // namespace dumb_math::performante_tests