cmake_minimum_required(VERSION 3.21)
project(benchmarking LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC 
    src/common.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        RLogSU
        
    INTERFACE
        random::variance_calc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set(BENCHMARKING_ENV_SETUP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/env_setup" CACHE INTERNAL "Path to benchmark env_setup")
set(BENCHMARKING_STAT_PLOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stat_plot" CACHE INTERNAL "Path to benchmark stat_plot")
set(BENCHMARK_CPU_ID "1" CACHE STRING "CPU core ID to pin benchmarks to (e.g., 0, 1, 2...)")
