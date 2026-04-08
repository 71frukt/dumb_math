cmake_minimum_required(VERSION 3.21)
project(logarithm_unittests LANGUAGES CXX)

enable_testing() 

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

find_library(GMP_LIB  gmp  REQUIRED)
find_library(MPFR_LIB mpfr REQUIRED)

add_executable(${PROJECT_NAME}
    src/test_special.cpp
    src/test_ulp_error.cpp
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        dumb_math_common
        dumb_math_internal
        
        tests_common

        logarithm

        ${GMP_LIB}
        ${MPFR_LIB}

        GTest::gtest_main
)

target_include_directories(logarithm_unittests 
    PRIVATE 
        "${CMAKE_CURRENT_BINARY_DIR}/../statistics/include"
)

add_dependencies(logarithm_unittests generate_plots)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})