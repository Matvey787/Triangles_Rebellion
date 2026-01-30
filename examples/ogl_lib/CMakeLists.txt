cmake_minimum_required(VERSION 3.28)
project(example1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OGL_LIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ogl_lib)
set(OGL_LIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/ogl_lib)

add_subdirectory(${OGL_LIB_SOURCE_DIR} ${OGL_LIB_BINARY_DIR})

# Example1 exe


add_executable(1_example
    ${CMAKE_CURRENT_SOURCE_DIR}/1_example.cpp
)

target_link_libraries(1_example
    PRIVATE
        ogl_lib
)

add_executable(2_intersections
    ${CMAKE_CURRENT_SOURCE_DIR}/2_intersections.cpp
)

target_link_libraries(2_intersections
    PRIVATE
        ogl_lib
        bvh_lib
)
