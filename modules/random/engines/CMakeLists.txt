cmake_minimum_required(VERSION 3.15)
project(engines LANGUAGES CXX)

add_subdirectory(minstd)
add_subdirectory(mrg32k3a)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE 
        RLogSU
        minstd
        mrg32k3a
)