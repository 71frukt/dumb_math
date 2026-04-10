add_subdirectory(generators/rnd_bit_generators)
add_subdirectory(generators/rnd_normal_generator)
add_subdirectory(generators/chi_sq_generator)
add_subdirectory(generators/bivariate_normal_generator)
add_subdirectory(test_battery)
add_subdirectory(variance_calc)

add_library(random INTERFACE)

target_link_libraries(random INTERFACE
    rnd_bit_generators
    rnd_normal_generator
    chi_sq_generator
    bivariate_normal_generator
    test_battery
    random::variance_calc
)