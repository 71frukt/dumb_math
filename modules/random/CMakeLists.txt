add_subdirectory(rnd_bit_generators)
add_subdirectory(rnd_normal_generator)
add_subdirectory(chi_sq_generator)
add_subdirectory(bivariate_normal_generator)
add_subdirectory(test_battery)

add_library(random INTERFACE)

target_link_libraries(random INTERFACE
    rnd_bit_generators
    rnd_normal_generator
    chi_sq_generator
    bivariate_normal_generator
    test_battery
)