cmake_minimum_required(VERSION 3.15)
project(uniformity_tests LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(uniformity_tests INTERFACE ~/boost_1_84_0)

target_link_libraries(${PROJECT_NAME} INTERFACE
    utils
    RLogSU
)