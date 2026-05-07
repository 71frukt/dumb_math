add_library(common STATIC 
    src/matrix_iostream.cpp
)

target_link_libraries(common
    PRIVATE
        assets::config

        matrix
        RLogSU
)

target_include_directories(common INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)