add_subdirectory(first_level_tests)
add_subdirectory(two_level_test)
add_subdirectory(uniformity_tests)
add_subdirectory(utils)

add_library(test_battery INTERFACE)

target_link_libraries(test_battery INTERFACE
    first_level_tests
    two_level_test
    uniformity_tests
)