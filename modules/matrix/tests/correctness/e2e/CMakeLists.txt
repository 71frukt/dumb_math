add_executable(generator
    src/generator.cpp
)

add_executable(matrix_tester
    src/tester.cpp
)

add_library(matrix_test_dependencies INTERFACE)

set(KEYS_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/keys")
set(TASKS_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/tasks")
set(CUR_SOURCE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}")

file(MAKE_DIRECTORY "${KEYS_DIR}")
file(MAKE_DIRECTORY "${TASKS_DIR}")

target_link_libraries(matrix_test_dependencies
    INTERFACE
        RLogSU

        matrix
        assets::config
        common
)

target_compile_definitions(matrix_test_dependencies
    INTERFACE
        "KEYS_DIR=\"${KEYS_DIR}\""
        "TASKS_DIR=\"${TASKS_DIR}\""
        "CUR_SOURCE_DIR=\"${CUR_SOURCE_DIR}\""
)
target_link_libraries(generator
    PRIVATE
        matrix_test_dependencies
)

target_link_libraries(matrix_tester
    PRIVATE
        matrix_test_dependencies
        GTest::gtest_main
)

#====================== генерация тестов ==========================================
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/e2e_tests_generated.stamp"
    COMMAND generator
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/e2e_tests_generated.stamp"
    DEPENDS generator
    COMMENT "Running generator to create e2e correctness tests..."
    VERBATIM
)

add_custom_target(run_e2e_generator
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/e2e_tests_generated.stamp"
)
add_dependencies(matrix_tester run_e2e_generator)
add_dependencies(run_e2e_generator generate_test_assets)
#====================== /генерация тестов =========================================

gtest_discover_tests(matrix_tester
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DISCOVERY_MODE PRE_TEST
)