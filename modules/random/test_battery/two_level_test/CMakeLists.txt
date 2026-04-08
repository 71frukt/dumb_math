cmake_minimum_required(VERSION 3.15)
project(two_level_test LANGUAGES CXX)

add_library(${PROJECT_NAME}
    src/two_level_tester.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(${PROJECT_NAME} PUBLIC ~/boost_1_84_0)

target_link_libraries(${PROJECT_NAME} PRIVATE
    utils
    RLogSU
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    first_level_tests
    uniformity_tests
)