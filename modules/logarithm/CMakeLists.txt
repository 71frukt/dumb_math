cmake_minimum_required(VERSION 3.21)
project(logarithm LANGUAGES CXX)

add_subdirectory(tools/lookup_table_builder)

#================= Lookup Table generation =========================
set(GENERATED_LUT "${CMAKE_CURRENT_SOURCE_DIR}/generated/lookup_table.hpp")

add_custom_command(
    OUTPUT "${GENERATED_LUT}"
    COMMAND $<TARGET_FILE:lookup_table_builder> --output "${GENERATED_LUT}" --table_size_exp 8
    
    DEPENDS $<TARGET_FILE:lookup_table_builder>
    COMMENT "Generating lookup table for logarithm..."
)
add_custom_target(generate_logarithm_lut DEPENDS "${GENERATED_LUT}")
#====================================================================

#================== public ==========================================
add_library(logarithm_common INTERFACE)
target_include_directories(logarithm_common INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
#====================================================================

add_library(${PROJECT_NAME} INTERFACE 
    include/logarithm/logarithm.hpp
)

add_dependencies(${PROJECT_NAME} generate_logarithm_lut)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        RLogSU
        dumb_math_common
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generated> 
)

#================= Tests =============================================
option(LN_BUILD_TESTING "Build ln tests" ON)

if(LN_BUILD_TESTING)
    add_subdirectory(tests)
endif()