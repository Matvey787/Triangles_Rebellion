cmake_minimum_required(VERSION 3.28)
project(ogllib LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


# GEO Lib

set(GEO_LIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../geo_lib)
set(GEO_LIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/geo_lib)

add_subdirectory(${GEO_LIB_SOURCE_DIR} ${GEO_LIB_BINARY_DIR})


# GLAD Lib

set(GLAD_LIB glad_lib)
add_library(${GLAD_LIB})
set(GLAD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/details/glad/src)
set(GLAD_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/details/glad/include)


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

set(GLM_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/details/glm)

target_include_directories(${GLM_LIB}
    INTERFACE
        ${GLM_INC_DIR}
)


# OGLWrap Lib

set(OGL_LIB ogl_lib)
add_library(${OGL_LIB})

file(GLOB OGL_MODULES 
    "${CMAKE_CURRENT_SOURCE_DIR}/details/*.cppm"
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
        geo_lib
)


