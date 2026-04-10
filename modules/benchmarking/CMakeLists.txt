cmake_minimum_required(VERSION 3.21)
project(benchmarking LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE 
    include/benchmarking/latency_test.hpp
    include/benchmarking/common.hpp
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        RLogSU
        random::variance_calc
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set(BENCHMARKING_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tools" CACHE INTERNAL "Path to benchmark scripts")
set(BENCHMARK_CPU_ID "1" CACHE STRING "CPU core ID to pin benchmarks to (e.g., 0, 1, 2...)")
