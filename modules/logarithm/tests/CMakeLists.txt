cmake_minimum_required(VERSION 3.21)
project(logarithm_tests LANGUAGES CXX)

#=================== tests_common ========================
add_library(tests_common INTERFACE)

find_library(GMP_LIB  gmp  REQUIRED)
find_library(MPFR_LIB mpfr REQUIRED)

target_include_directories(tests_common INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

target_link_libraries(tests_common
    INTERFACE
        dumb_math_internal
        logarithm
        ${GMP_LIB}
        ${MPFR_LIB}
)
#=========================================================


add_subdirectory(unit)
add_subdirectory(statistics)