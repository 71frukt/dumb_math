cmake_minimum_required(VERSION 3.15)
project(engines LANGUAGES CXX)

add_subdirectory(minstd)
add_subdirectory(mrg32k3a)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME}
    INTERFACE 
        RLogSU
        minstd
        mrg32k3a
)