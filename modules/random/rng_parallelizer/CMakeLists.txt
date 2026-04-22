project(rng_parallelizer LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC
    src/cores_info_getter.cpp
)
add_library(random::rng_parallelizer ALIAS rng_parallelizer)


target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    RLogSU
)

target_compile_options(${PROJECT_NAME} 
    PRIVATE 
        -pthread
)