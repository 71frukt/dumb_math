project(matrix_tests LANGUAGES CXX)

#===================== configurating assets/config =================================
set(RECTANGLE_MATRICIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets/rectangle")
set(SQUARE_MATRICIES_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/assets/square")

file(MAKE_DIRECTORY "${RECTANGLE_MATRICIES_DIR}")
file(MAKE_DIRECTORY "${SQUARE_MATRICIES_DIR}")


set(SQUARE_MATRICIES_MAX_SIZE      3000)
set(SQUARE_MATRICIES_BLOCK_SIZE    2   )
set(SQUARE_MATRICIES_SIZE_MUL_STEP 1.25)

set(RECTANGLE_MATRICIES_MAX_COLS      1024)
set(RECTANGLE_MATRICIES_MAX_ROWS      1024)
set(RECTANGLE_MATRICIES_BLOCK_SIZE    2   )
set(RECTANGLE_MATRICIES_SIZE_MUL_STEP 1.7 )

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
add_subdirectory(assets/generator)

#====================== генерация ассетов ==========================================
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/assets_generated.stamp"
    COMMAND tests_assets_generator
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/assets_generated.stamp"
    DEPENDS tests_assets_generator
    COMMENT "Running tests_assets_generator to create matrices..."
    VERBATIM
)

add_custom_target(generate_test_assets
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/assets_generated.stamp"
)
#====================== /генерация ассетов =========================================

# Добавляем зависимые сабдиректории после объявления таргета генерации
add_subdirectory(performance)
add_subdirectory(correctness/e2e)