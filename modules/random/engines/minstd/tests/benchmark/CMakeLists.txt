project(minstd_performance_test LANGUAGES CXX)

set(BENCHMARK_ITERATIONS 500000000)

add_executable(throughput_scalar_one_thread
    throughput.cpp
)

target_link_libraries(throughput_scalar_one_thread
    PRIVATE
        benchmarking
)
      
#==== Настройка CTest Fixtures (Окружение для бенчмарков) ================

add_test(NAME EnvSetup 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/set_performance.sh)
set_tests_properties(EnvSetup PROPERTIES FIXTURES_SETUP PerfFixture)

add_test(NAME EnvTeardown 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/restore_cpu_state.sh)
set_tests_properties(EnvTeardown PROPERTIES FIXTURES_CLEANUP PerfFixture)

#==== Настройка CTest Fixtures (Окружение для бенчмарков) ================


# ===================== Scalar One Thread Test =====================================

add_test(NAME throughput_scalar_one_thread_test 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/run_max_priority.sh
         $<TARGET_FILE:throughput_scalar_one_thread>
         ${BENCHMARK_CPU_ID}
         ${BENCHMARK_ITERATIONS}
)
         
set_tests_properties(throughput_scalar_one_thread_test
    PROPERTIES
        FIXTURES_REQUIRED PerfFixture
        RUN_SERIAL TRUE
)

# ===================== /Scalar One Thread Test ====================================
