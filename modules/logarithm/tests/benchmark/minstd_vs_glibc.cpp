#include <cmath>
#include <vector>
#include <random>
#include <iostream>

#include "benchmarking/common.hpp"
#include "logarithm/logarithm.hpp"
#include "benchmarking/benchmarking.hpp"

template <typename T>
std::vector<T> GenerateData(size_t size, T min_val, T max_val)
{
    std::vector<T> data(size);
    std::mt19937 gen(42);
    std::uniform_real_distribution<T> dist(min_val, max_val);
    
    for (auto& d : data)
    {
        d = dist(gen);
    }
    
    return data;
}

template <typename DataT, typename LnFuncT>
dumb_math::benchmarking::ResultT RunBenchmark(LnFuncT&& ln_func, const std::vector<DataT>& data)
{
    auto array_handle = [&data, &ln_func]()
    {
        for (const DataT& v : data)
        {
            ln_func(v);
        }
    };

    return dumb_math::benchmarking::TestThroughput(array_handle, 10);
}


template <typename T>
void RunBenchmarksForType(const std::string& type_name)
{
    const size_t data_size = 20000;

    auto data_normal    = GenerateData<T>(data_size, 0.1,  1000.0);
    auto data_near_one  = GenerateData<T>(data_size, 0.99, 1.01);
    auto data_huge      = GenerateData<T>(data_size, 1e15, 1e30);
    auto data_subnormal = GenerateData<T>(data_size, 
                                          std::numeric_limits<T>::denorm_min(), 
                                          std::numeric_limits<T>::min());

    using namespace dumb_math::benchmarking;
    
    #define BENCH_PAIR(DataArray, ScenarioName) {                                                           \
        std::cout << "--- " << type_name << " | " << ScenarioName << " ---\n";                              \
        /* Используем лямбды вместо жесткого каста указателей на функции */                                 \
        auto my_ln    = [](T x) { return dumb_math::logarithm::ln<T>(x); };                                 \
        auto glibc_ln = [](T x) { return std::log(x); };                                                    \
        ResultT myln_res  = RunBenchmark<T>(my_ln, DataArray);                                              \
        ResultT glibc_res = RunBenchmark<T>(glibc_ln, DataArray);                                           \
        std::cout << "my ln: " << myln_res.average  << " +- " << myln_res.standard_deviation << "\n";       \
        std::cout << "glibc: " << glibc_res.average << " +- " << glibc_res.standard_deviation << "\n\n";    \
    }    

    BENCH_PAIR(data_normal, "Normal Range [0.1, 1000]");
    BENCH_PAIR(data_near_one, "Near One [0.99, 1.01]");
    BENCH_PAIR(data_huge, "Huge Values");
    BENCH_PAIR(data_subnormal, "Subnormal (Denormals)");
    
    #undef BENCH_PAIR
}

int main() {
    RunBenchmarksForType<float>("FLOAT");
    std::cout << "\n=================================\n\n";
    RunBenchmarksForType<double>("DOUBLE");
    
    return 0;
}