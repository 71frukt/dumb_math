project(mrg32k3a LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE
    include/engines/mrg32k3a/generator.hpp
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    RLogSU
)
