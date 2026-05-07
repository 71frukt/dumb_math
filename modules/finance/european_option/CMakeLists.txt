enable_testing()

project(mc_performance_test LANGUAGES CXX)

add_test(NAME EnvSetup 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/set_performance.sh)
set_tests_properties(EnvSetup PROPERTIES FIXTURES_SETUP PerfFixture)

add_test(NAME EnvTeardown 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/restore_cpu_state.sh)
set_tests_properties(EnvTeardown PROPERTIES FIXTURES_CLEANUP PerfFixture)



# ===================== Scalar One Thread Test =====================================

add_executable(scalar_one_thread src/scalar_one_thread.cpp)
target_link_libraries(scalar_one_thread PRIVATE 
    benchmarking 
    minstd 
    rng_parallelizer
)

target_include_directories(scalar_one_thread
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_test(NAME mc_scalar_one_thread_run
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/run_max_priority.sh
         $<TARGET_FILE:scalar_one_thread>
         ${BENCHMARK_CPU_ID}
)
         
set_tests_properties(mc_scalar_one_thread_run
    PROPERTIES
        FIXTURES_REQUIRED PerfFixture
        RUN_SERIAL TRUE
)

# ===================== /Scalar One Thread Test ====================================


# ===================== Scalar Multi Thread Test =====================================
add_executable(scalar_multithread_test src/scalar_multithread.cpp )
target_link_libraries(scalar_multithread_test PRIVATE 
    benchmarking
    random
)

target_include_directories(scalar_multithread_test
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_test(NAME scalar_multithread_run
         COMMAND sudo $<TARGET_FILE:scalar_multithread_test>
)

set_tests_properties(scalar_multithread_run
    PROPERTIES
        FIXTURES_REQUIRED PerfFixture
        RUN_SERIAL TRUE
)
# ===================== Scalar Multi Thread Test =====================================




# ===================== Vector Multi Thread Test =====================================
add_executable(vector_multithread_test src/vector_multithread.cpp )
target_link_libraries(vector_multithread_test PRIVATE 
    benchmarking
    random
    mvec m
)

target_include_directories(vector_multithread_test
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_options(vector_multithread_test PRIVATE -march=native)

add_test(NAME vector_multithread_run
         COMMAND sudo $<TARGET_FILE:vector_multithread_test>
)

set_tests_properties(vector_multithread_run
    PROPERTIES
        FIXTURES_REQUIRED PerfFixture
        RUN_SERIAL TRUE
)
# ===================== Vector Multi Thread Test =====================================
