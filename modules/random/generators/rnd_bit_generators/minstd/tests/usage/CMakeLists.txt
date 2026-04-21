cmake_minimum_required(VERSION 3.21)
project(usage_performance_test LANGUAGES CXX)


add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        benchmarking
)


set(DATA_DIR             "${CMAKE_CURRENT_BINARY_DIR}" )
set(LATENCY_TESTS_DIR    "${DATA_DIR}/latency/data"    )
set(LATENCY_PLOT_DIR     "${DATA_DIR}/latency/plots"   )
set(THROUGHPUT_TESTS_DIR "${DATA_DIR}/throughput/data" )
set(THROUGHPUT_PLOT_DIR  "${DATA_DIR}/throughput/plots")

file(MAKE_DIRECTORY "${LATENCY_TESTS_DIR}"   )
file(MAKE_DIRECTORY "${LATENCY_PLOT_DIR}"    )
file(MAKE_DIRECTORY "${THROUGHPUT_TESTS_DIR}")
file(MAKE_DIRECTORY "${THROUGHPUT_PLOT_DIR}" )

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        "LATENCY_TESTS_DIR=\"${LATENCY_TESTS_DIR}\""        
        "THROUGHPUT_TESTS_DIR=\"${THROUGHPUT_TESTS_DIR}\""        
        "PERFORMANCE_TEST_TIME_LIMIT_S=${PERFORMANCE_TEST_TIME_LIMIT_S}"
)

#==== Настройка CTest Fixtures (Окружение для бенчмарков) ================

add_test(NAME UsageEnvSetup 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/set_performance.sh)
set_tests_properties(UsageEnvSetup PROPERTIES FIXTURES_SETUP UsagePerfFixture)

add_test(NAME UsageEnvTeardown 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/restore_cpu_state.sh)
set_tests_properties(UsageEnvTeardown PROPERTIES FIXTURES_CLEANUP UsagePerfFixture)

add_test(NAME UsageRunPerfTest 
         COMMAND sudo ${BENCHMARKING_ENV_SETUP_DIR}/run_max_priority.sh $<TARGET_FILE:${PROJECT_NAME}> ${BENCHMARK_CPU_ID})
         
set_tests_properties(UsageRunPerfTest PROPERTIES FIXTURES_REQUIRED UsagePerfFixture)

#------------------------- График ------------------------------------------------
find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_test(NAME UsagePlotLatency 
         COMMAND bash -c "${Python3_EXECUTABLE} ${BENCHMARKING_STAT_PLOT_DIR}/spline.py ${LATENCY_PLOT_DIR} ${LATENCY_TESTS_DIR}/*.csv")

set_tests_properties(UsagePlotLatency PROPERTIES 
                     DEPENDS UsageRunPerfTest
                     FIXTURES_REQUIRED UsagePerfFixture)

add_test(NAME UsagePlotThroughput 
         COMMAND bash -c "${Python3_EXECUTABLE} ${BENCHMARKING_STAT_PLOT_DIR}/spline.py ${THROUGHPUT_PLOT_DIR} ${THROUGHPUT_TESTS_DIR}/*.csv")

set_tests_properties(UsagePlotThroughput PROPERTIES 
                     DEPENDS UsageRunPerfTest
                     FIXTURES_REQUIRED UsagePerfFixture)