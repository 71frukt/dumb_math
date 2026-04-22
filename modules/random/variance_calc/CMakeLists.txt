project(variance_calc LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE
    include/variance_calc/variance_calculator.hpp
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)