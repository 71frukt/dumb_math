add_library(clogf STATIC clogf.cpp)

target_link_libraries(clogf PRIVATE
    dumb_math
)

target_include_directories(clogf
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)