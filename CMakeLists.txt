cmake_minimum_required(VERSION 3.17)
project(LIRS_Cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD "cxx_std_20")

option(GTEST "Build tests" OFF)
option(SANITIZE "Enable sanitizers" OFF)

set(SANITIZER_FLAGS "-fsanitize=address,undefined,leak")
set(DEBUG_FLAGS "-Wextra -Wall -Wpedantic")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, defaulting to Debug")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build.")
endif()

if(SANITIZE)
    message(STATUS "Sanitizers enabled")
    list(APPEND DEBUG_FLAGS ${SANITIZER_FLAGS})
endif()

# --- Include directories -----------------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/inc)

# -------------------------------------------------------------------------------------------------
# Double lib
set(DOUBLE_LIB double INTERFACE)
add_library(${DOUBLE_LIB})


target_include_directories(${DOUBLE_LIB}
    INTERFACE
        libs/Double_Lib/inc
)

target_compile_features(${DOUBLE_LIB} INTERFACE ${CXX_STANDARD})


# -------------------------------------------------------------------------------------------------
# Point lib
set(POINT_LIB point)
add_library(${POINT_LIB} INTERFACE)

target_include_directories(${POINT_LIB}
    INTERFACE
        libs/Geo_Lib/Point/inc
)

target_link_libraries(${POINT_LIB}
    INTERFACE
        ${DOUBLE_LIB}
)

target_compile_features(${POINT_LIB} INTERFACE ${CXX_STANDARD})


# -------------------------------------------------------------------------------------------------
# Vector lib
set(VECTOR_LIB vector)
add_library(${VECTOR_LIB} INTERFACE)

target_include_directories(${VECTOR_LIB}
    INTERFACE
        libs/Geo_Lib/Vector/inc
)

target_link_libraries(${VECTOR_LIB}
    INTERFACE
        ${POINT_LIB}
        ${DOUBLE_LIB}
)

target_compile_features(${VECTOR_LIB} INTERFACE ${CXX_STANDARD})


# -------------------------------------------------------------------------------------------------
# Line lib
set(LINE_LIB line)
add_library(${LINE_LIB} INTERFACE)

target_include_directories(${LINE_LIB}
    INTERFACE
        libs/Geo_Lib/Line/inc
)

target_link_libraries(${LINE_LIB}
    INTERFACE
        ${POINT_LIB} 
        ${VECTOR_LIB}
        ${DOUBLE_LIB}
)

target_compile_features(${LINE_LIB} INTERFACE ${CXX_STANDARD})


# -------------------------------------------------------------------------------------------------
# Triangle lib
set(TRIANGLE_LIB triangle)
add_library(${TRIANGLE_LIB} INTERFACE)

target_include_directories(${TRIANGLE_LIB}
    INTERFACE
        libs/Geo_Lib/Triangle/inc
)

target_link_libraries(${TRIANGLE_LIB}
    INTERFACE
        ${POINT_LIB} 
        ${VECTOR_LIB}
        ${LINE_LIB}
        ${DOUBLE_LIB}
)

target_compile_features(${TRIANGLE_LIB} INTERFACE ${CXX_STANDARD})

# -------------------------------------------------------------------------------------------------
# Geo (point vector, line, triangle libs)
set(GEO_LIB geo)
add_library(${GEO_LIB} INTERFACE)

target_link_libraries(${GEO_LIB}
    INTERFACE
        ${POINT_LIB}
        ${VECTOR_LIB}
        ${LINE_LIB}
        ${TRIANGLE_LIB}
        ${DOUBLE_LIB}
)

# -------------------------------------------------------------------------------------------------
# BVH lib
set(BVH_LIB bvh)
add_library(${BVH_LIB} INTERFACE)

target_include_directories(${BVH_LIB}
    INTERFACE
        libs/BVH_Lib/inc
)

target_link_libraries(${BVH_LIB}
    INTERFACE
        ${GEO_LIB}
        ${DOUBLE_LIB}
)

target_compile_features(${BVH_LIB} INTERFACE ${CXX_STANDARD})

# -------------------------------------------------------------------------------------------------
# Executable
set(OUT_EXE out)
add_executable(${OUT_EXE}
    src/main.cpp
)

target_compile_features(${OUT_EXE} PRIVATE ${CXX_STANDARD})

target_link_libraries(${OUT_EXE}
    PRIVATE
        ${BVH_LIB}
        ${GEO_LIB}
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${OUT_EXE} PRIVATE ${DEBUG_FLAGS})
endif()

add_subdirectory(tester)