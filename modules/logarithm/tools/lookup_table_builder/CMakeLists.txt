cmake_minimum_required(VERSION 3.21)
project(lookup_table_builder LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/lookup_table_builder.cpp
    src/lookup_table_printer.cpp

    tools/cmd_options/cmd_options.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tools>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib>

        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        RLogSU
        dumb_math_common
        dumb_math_internal
 )