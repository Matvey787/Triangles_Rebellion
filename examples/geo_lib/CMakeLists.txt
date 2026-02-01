cmake_minimum_required(VERSION 3.28)
project(example1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Geo lib (BVH Lib also exists here)

set(GEO_LIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../geo_lib)
set(GEO_LIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/geo_lib)

add_subdirectory(${GEO_LIB_SOURCE_DIR} ${GEO_LIB_BINARY_DIR})





# Example1 exe

set(PROGRAM_EXE intersection)

add_executable(${PROGRAM_EXE}
    ${CMAKE_CURRENT_SOURCE_DIR}/intersection.cpp
)

target_link_libraries(${PROGRAM_EXE}
    PRIVATE
        geo_lib
        bvh_lib
)


