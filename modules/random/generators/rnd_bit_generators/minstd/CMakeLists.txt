project(random_minstd LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE
    include/rnd_bit_generators/minstd/minstd.hpp
)

add_library(random::minstd ALIAS random_minstd)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    RLogSU
)
