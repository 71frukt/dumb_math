project(minstd_performance_test LANGUAGES CXX)

set(BENCHMARK_ITERATIONS 50000000)
# set(BENCHMARK_ITERATIONS 50)

set(BENCHMARK_TARGETS
    throughput_scalar_one_thread
    throughput_scalar_multi_thread
    throughput_vector_one_thread
    throughput_vector_multi_thread
)

foreach(TARGET_NAME IN LISTS BENCHMARK_TARGETS)

    add_executable(${TARGET_NAME}
        ${TARGET_NAME}.cpp
    )   

    target_link_libraries(${TARGET_NAME} PRIVATE benchmarking)
endforeach()
      
#==== Настройка CTest Fixtures (Окружение для бенчмарков) ================

if(NOT DEFINED BENCHMARKING_ENV_SETUP_DIR OR BENCHMARKING_ENV_SETUP_DIR STREQUAL "")
    message(FATAL_ERROR 
        "BENCHMARKING_ENV_SETUP_DIR is not defined or empty. "
        "Check add_subdirectory() order: benchmarking must precede random."
    )
endif()

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


# ===================== Scalar Multi Thread Test =====================================

add_test(NAME throughput_scalar_multi_thread_test 
         COMMAND sudo $<TARGET_FILE:throughput_scalar_multi_thread>
         ${BENCHMARK_ITERATIONS}
)
         
set_tests_properties(throughput_scalar_multi_thread_test
    PROPERTIES
        FIXTURES_REQUIRED PerfFixture
        RUN_SERIAL TRUE
)

# ===================== /Scalar Multi Thread Test ====================================

# ===================== Vector One Thread Test =====================================

add_test(NAME throughput_vector_one_thread_test 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/run_max_priority.sh
         $<TARGET_FILE:throughput_vector_one_thread>
         ${BENCHMARK_CPU_ID}
         ${BENCHMARK_ITERATIONS}
)
         
set_tests_properties(throughput_vector_one_thread_test
    PROPERTIES
        FIXTURES_REQUIRED PerfFixture
        RUN_SERIAL TRUE
)

# ===================== /Vector One Thread Test ====================================

# ===================== Vector Multi Thread Test =====================================

add_test(NAME throughput_vector_multi_thread_test 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/run_max_priority.sh
         $<TARGET_FILE:throughput_vector_multi_thread>
         ${BENCHMARK_CPU_ID}
         ${BENCHMARK_ITERATIONS}
)
         
set_tests_properties(throughput_vector_multi_thread_test
    PROPERTIES
        FIXTURES_REQUIRED PerfFixture
        RUN_SERIAL TRUE
)

# ===================== /Vector Multi Thread Test ====================================
