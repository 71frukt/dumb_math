#pragma once

#include <functional>
#include <cstdint>
#include <string>

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

void ExportResultsToCSV(std::string curve_name, const std::vector<std::pair<double, ResultT>>& data, const std::string& filename);


namespace detail {
    
template <class T>
__attribute__((always_inline)) inline void DoNotOptimizeLatency(const T& value)
{
    asm volatile("" : : "r,m"(value) : "memory");
    _mm_lfence();
}

template <class T>
__attribute__((always_inline)) inline void DoNotOptimizeThroughput(const T& value)
{
    asm volatile("" : : "r,m"(value));
}

__attribute__((always_inline)) inline void ClobberMemory()
{
    asm volatile("" ::: "memory");
}

template <typename Func>
__attribute__((always_inline)) inline void RunAndProtectLatency(Func&& f)
{
    if constexpr (std::is_void_v<std::invoke_result_t<Func>>)
    {
        f();
        ClobberMemory();
        _mm_lfence();
    }
    
    else
    {
        DoNotOptimizeLatency(f());
    }
}

template <typename Func>
__attribute__((always_inline)) inline void RunAndProtectThroughput(Func&& f)
{
    if constexpr (std::is_void_v<std::invoke_result_t<Func>>)
    {
        f();
    }
    else
    {
        DoNotOptimizeThroughput(f());
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

void CheckCpuGovernor();

} // namespace detail

} // namespace dumb_math::performante_tests