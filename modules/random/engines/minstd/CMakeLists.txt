project(minstd LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC
    src/generator_vect.cpp
    src/tools/mersenne_mod_exp.cpp
)

target_compile_options(${PROJECT_NAME} 
    PUBLIC 
        -mavx2
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    RLogSU
    rng_parallelizer
)

#================= Tests =============================================
option(MINSTD_BUILD_TESTING "Build MINSTD tests" ON)

if(MINSTD_BUILD_TESTING)
    add_subdirectory(tests/benchmark)
    add_subdirectory(tests/validation)
endif()