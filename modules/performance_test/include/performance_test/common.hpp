#pragma once

#include <cstdint>
#include <functional>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#include <cpuid.h>
#endif

namespace dumb_math::performante_test {

struct ResultT
{
    double average;
    double variance;
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

} // namespace detail

} // namespace dumb_math::performante_tests