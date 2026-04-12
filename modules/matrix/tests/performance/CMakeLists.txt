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
file(MAKE_DIRECTORY "${DATA_DIR}")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    DATA_DIR="${DATA_DIR}"
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