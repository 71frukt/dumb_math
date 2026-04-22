add_subdirectory(bivariate_normal)
add_subdirectory(chi_square)
add_subdirectory(normal)

add_library(distributions INTERFACE)

target_include_directories(distributions INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)