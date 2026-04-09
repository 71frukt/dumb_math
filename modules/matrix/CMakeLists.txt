cmake_minimum_required(VERSION 3.21)
project(matrix LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC 
    src/matrix.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        RLogSU
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

#================= Tests =============================================
# option(LN_BUILD_TESTING "Build ln tests" ON)

# if(LN_BUILD_TESTING)
#     add_subdirectory(tests)
# endif()