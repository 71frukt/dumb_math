cmake_minimum_required(VERSION 3.15)
project(dumb_math LANGUAGES CXX)

###=========================================== DEPENDENCIES-RLogSU ==========================================
find_package(RLogSU QUIET)

if(NOT RLogSU_FOUND)
    message(STATUS "RLogSU was not found in the system. We use a local copy in lib/RLogSU (or download it if it is not available)...")
    
    include(FetchContent)
    FetchContent_Declare(
        RLogSU
        GIT_REPOSITORY https://github.com/71frukt/RLogSU.git
        GIT_TAG        main
        SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/lib/RLogSU
    )

    # add_subdirectory тут автоматически
    FetchContent_MakeAvailable(RLogSU)
else()
    message(STATUS "is found in the system, using the system version.")
endif()
###=========================================== /DEPENDENCIES-RLogSU =========================================


#============================================= GTESTS =======================================================
include(CTest) 

if(BUILD_TESTING)
    include(FetchContent)
    set(FETCHCONTENT_QUIET OFF)

    # Заодно решаем твою проблему с CI: используем Git вместо URL
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()
#============================================= /GTESTS ======================================================


#================== public ===============================
add_library(dumb_math_common INTERFACE)
target_include_directories(dumb_math_common INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

#================= private ===============================
add_library(dumb_math_internal INTERFACE)
target_include_directories(dumb_math_internal INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
)

#========================================= MODULES ====================================================
add_subdirectory(modules/logarithm)
add_subdirectory(modules/random)
add_subdirectory(modules/matrix)
#========================================= /MODULES ===================================================

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    dumb_math_common
    
    logarithm
    random
    matrix
)

