cmake_minimum_required(VERSION 3.15)
project(first_level_tests LANGUAGES CXX)

add_library(${PROJECT_NAME}
    src/disjoint_vectors.cpp
    src/fourier_tester.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    utils
    RLogSU
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${Boost_INCLUDE_DIRS}
)
