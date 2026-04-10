cmake_minimum_required(VERSION 3.21)
project(performance_test LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE 
    include/performance_test/common.hpp
    include/performance_test/latency_test.hpp
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