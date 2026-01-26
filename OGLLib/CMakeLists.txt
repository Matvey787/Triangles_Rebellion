cmake_minimum_required(VERSION 3.28)
project(ogl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, defaulting to Debug")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build.")
endif()


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


# GLAD Lib
set(GLAD_LIB glad_lib)
add_library(${GLAD_LIB})
set(GLAD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/OGLWrap/glad/src)
set(GLAD_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/OGLWrap/glad/include)


file(GLOB GLAD_SOURCES "${GLAD_SRC_DIR}/*.c")

target_sources(${GLAD_LIB}
    PRIVATE
        ${GLAD_SOURCES}
)

target_include_directories(${GLAD_LIB}
    PUBLIC
        ${GLAD_INC_DIR}
)


# GLM Lib 

set(GLM_LIB glm_lib)
add_library(${GLM_LIB} INTERFACE)

set(GLM_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/OGLWrap/glm)

target_include_directories(${GLM_LIB}
    INTERFACE
        ${GLM_INC_DIR}
)


# OGLWrap Lib

set(OGL_LIB ogl_lib)
add_library(${OGL_LIB})

file(GLOB OGL_MODULES 
    "${CMAKE_CURRENT_SOURCE_DIR}/OGLWrap/details/*.cppm"
    "${CMAKE_CURRENT_SOURCE_DIR}/OGLWrap/*.cppm"
)

target_sources(${OGL_LIB}
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${OGL_MODULES}
)

target_link_libraries(${OGL_LIB}
    PUBLIC
        ${GLAD_LIB}
        ${GLM_LIB}
        glfw
)

target_include_directories(${OGL_LIB}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
)


# Example1 exe

set(EXAMPLE1_EXE example1)

add_executable(${EXAMPLE1_EXE}
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/example1.cpp
)

target_link_libraries(${EXAMPLE1_EXE}
    PUBLIC
        ${OGL_LIB}
)
