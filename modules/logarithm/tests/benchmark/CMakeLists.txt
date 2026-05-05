project(logarithm_benchmark LANGUAGES CXX)


add_executable(${PROJECT_NAME}
    minstd_vs_glibc.cpp
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        logarithm
        benchmarking

        GTest::gtest_main
)


#==== Настройка CTest Fixtures (Окружение для бенчмарков) ================

add_test(NAME EnvSetup 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/set_performance.sh)
set_tests_properties(EnvSetup PROPERTIES FIXTURES_SETUP PerfFixture)

add_test(NAME EnvTeardown 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/restore_cpu_state.sh)
set_tests_properties(EnvTeardown PROPERTIES FIXTURES_CLEANUP PerfFixture)

add_test(NAME MinstdRunPerfTest 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/run_max_priority.sh $<TARGET_FILE:${PROJECT_NAME}> ${BENCHMARK_CPU_ID})
         
set_tests_properties(MinstdRunPerfTest PROPERTIES FIXTURES_REQUIRED PerfFixture)