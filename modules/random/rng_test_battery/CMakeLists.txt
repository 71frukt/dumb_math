add_subdirectory(utils)

add_library(rng_test_battery STATIC
    source/first_level_tests/disjoint_vectors.cpp
    source/first_level_tests/fourier_tester.cpp
    source/two_level_test/two_level_tester.cpp
)

target_link_libraries(rng_test_battery PUBLIC
    utils
)

target_include_directories(rng_test_battery PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
