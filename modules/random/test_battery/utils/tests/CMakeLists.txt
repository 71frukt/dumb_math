enable_testing()

cmake_minimum_required(VERSION 3.21)
project(utils_unittests LANGUAGES CXX)

add_executable(${PROJECT_NAME} src/test_fft.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        utils
        GTest::gtest_main
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

gtest_discover_tests(${PROJECT_NAME})