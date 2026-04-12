cmake_minimum_required(VERSION 3.21)
project(matrix_performance_test LANGUAGES CXX)


add_executable(${PROJECT_NAME}
    src/latency.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        matrix

        RLogSU
        benchmarking
        GTest::gtest_main
)

set(DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(LATENCY_TESTS_DIR "${DATA_DIR}/latency")
set(LATENCY_PLOT_FILE "${LATENCY_TESTS_DIR}/latency_plot.png")

file(MAKE_DIRECTORY "${LATENCY_TESTS_DIR}")

set(PERFORMANCE_TEST_TIME_LIMIT_S 3)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        "DATA_DIR=\"${DATA_DIR}\""
        "LATENCY_TESTS_DIR=\"${LATENCY_TESTS_DIR}\""        
        "PERFORMANCE_TEST_TIME_LIMIT_S=${PERFORMANCE_TEST_TIME_LIMIT_S}"
)

#==== Настройка CTest Fixtures (Окружение для бенчмарков) ================

add_test(NAME MatrixEnvSetup 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/set_performance.sh)
set_tests_properties(MatrixEnvSetup PROPERTIES FIXTURES_SETUP MatrixPerfFixture)

add_test(NAME MatrixEnvTeardown 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/restore_cpu_state.sh)
set_tests_properties(MatrixEnvTeardown PROPERTIES FIXTURES_CLEANUP MatrixPerfFixture)

add_test(NAME RunMatrixPerfTest 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/run_max_priority.sh $<TARGET_FILE:${PROJECT_NAME}> ${BENCHMARK_CPU_ID})
         
set_tests_properties(RunMatrixPerfTest PROPERTIES FIXTURES_REQUIRED MatrixPerfFixture)

#-------------------------График------------------------------------------------
find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_test(NAME PlotMatrixLatency 
         COMMAND ${Python3_EXECUTABLE} ${BENCHMARKING_STAT_PLOT_DIR}/spline.py 
                 ${LATENCY_PLOT_FILE}
                 ${LATENCY_TESTS_DIR}/DumbMul0.csv
                 ${LATENCY_TESTS_DIR}/DumbMul1.csv
                 ${LATENCY_TESTS_DIR}/OptMul0.csv 
                 ${LATENCY_TESTS_DIR}/OptMul1.csv 
                 ${LATENCY_TESTS_DIR}/BlockMul0.csv)

set_tests_properties(PlotMatrixLatency PROPERTIES 
                     DEPENDS RunMatrixPerfTest
                     FIXTURES_REQUIRED MatrixPerfFixture)