add_subdirectory(rng_parallelizer)
add_subdirectory(rng_test_battery)
add_subdirectory(engines)
add_subdirectory(distributions)
add_subdirectory(variance_calc)

add_library(random INTERFACE)
add_library(dumb_math::random ALIAS random)

target_link_libraries(random INTERFACE
    rng_parallelizer
    rng_test_battery
    engines
    distributions
    variance_calc
)