cmake_minimum_required(VERSION 3.28)
project(example1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OGL_LIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ogl_lib)
set(OGL_LIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/ogl_lib)

add_subdirectory(${OGL_LIB_SOURCE_DIR} ${OGL_LIB_BINARY_DIR})

# Example1 exe

set(EXAMPLE1_EXE example1)

add_executable(${EXAMPLE1_EXE}
    ${CMAKE_CURRENT_SOURCE_DIR}/example1.cpp
)

target_link_libraries(${EXAMPLE1_EXE}
    PRIVATE
        ogl_lib
)
