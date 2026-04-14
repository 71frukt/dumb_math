project(matrix_tests LANGUAGES CXX)

#===================== configurating assets/config =================================
set(RECTANGLE_MATRICIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets/rectangle")
set(SQUARE_MATRICIES_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/assets/square")

file(MAKE_DIRECTORY "${RECTANGLE_MATRICIES_DIR}")
file(MAKE_DIRECTORY "${SQUARE_MATRICIES_DIR}")


set(SQUARE_MATRICIES_MAX_SIZE   64)
set(SQUARE_MATRICIES_BLOCK_SIZE 2)

set(RECTANGLE_MATRICIES_MAX_COLS   16)
set(RECTANGLE_MATRICIES_MAX_ROWS   16)
set(RECTANGLE_MATRICIES_BLOCK_SIZE 2)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/assets/include/assets/config.hpp"
)

add_library(assets_config INTERFACE)
add_library(assets::config ALIAS assets_config)

target_include_directories(assets_config
    INTERFACE
        "${CMAKE_CURRENT_BINARY_DIR}/assets/include"
)
#===================== /configurating assets/config ================================

add_subdirectory(common)
add_subdirectory(performance)
add_subdirectory(assets/generator)
