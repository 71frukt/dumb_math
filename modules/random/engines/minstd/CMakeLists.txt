project(minstd LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE
    include/engines/minstd/generator_scalar.hpp
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    RLogSU
)
