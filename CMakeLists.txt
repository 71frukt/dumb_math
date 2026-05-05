cmake_minimum_required(VERSION 3.15)
project(dumb_math LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###=========================================== DEPENDENCIES-RLogSU ==========================================
find_package(RLogSU QUIET)

if(NOT RLogSU_FOUND)
    # Проверяем, есть ли уже локальная копия В ФАЙЛОВОЙ СИСТЕМЕ
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/RLogSU/CMakeLists.txt)
        message(STATUS "Using local RLogSU from lib/RLogSU")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/RLogSU)

    else()
        message(STATUS "Downloading RLogSU from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
            RLogSU
            GIT_REPOSITORY https://github.com/71frukt/RLogSU.git
            GIT_TAG        main
        )
        FetchContent_MakeAvailable(RLogSU)
    endif()
    
else()
    message(STATUS "RLogSU found in system, using system version.")
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


# защита от запуска бенчмарков без оптимизаций
if(BUILD_TESTING)
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Set CMAKE_BUILD_TYPE in 'Release' for correct benchmarking.")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "WARNING: Build in Debug mode! Benchmarking results (TestLatency) will be distorted and make no sense.")
    endif()
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
add_subdirectory(modules/benchmarking)
add_subdirectory(modules/random)
add_subdirectory(modules/logarithm)
add_subdirectory(modules/matrix)
add_subdirectory(modules/finance/european_option)
#========================================= /MODULES ===================================================

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        dumb_math_common
        
        benchmarking
        random
        logarithm
        matrix
)
