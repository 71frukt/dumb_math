cmake_minimum_required(VERSION 3.21)
project(matrix_correctness_test LANGUAGES CXX)


add_executable(${PROJECT_NAME}
    src/correctness.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        matrix

        RLogSU
        GTest::gtest_main
)