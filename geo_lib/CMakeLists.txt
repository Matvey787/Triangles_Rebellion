cmake_minimum_required(VERSION 3.28)
project(LIRS_Cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------------------------------------
# Double lib
set(DOUBLE_LIB double_lib)
add_library(${DOUBLE_LIB} INTERFACE)

target_include_directories(${DOUBLE_LIB}
    INTERFACE
        double_lib/inc/
)

target_compile_features(${DOUBLE_LIB} INTERFACE ${CXX_STANDARD})

# -------------------------------------------------------------------------------------------------
# Geo (point, vector, line, triangle libs)
set(GEO_LIB geo_lib)
add_library(${GEO_LIB})

file(GLOB_RECURSE GEO_MODULES 
    "${CMAKE_CURRENT_SOURCE_DIR}/geo_lib/*.cppm"
)

message(STATUS "Geo modules: ${GEO_MODULES}")

target_sources(${GEO_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${GEO_MODULES}
)


target_link_libraries(${GEO_LIB}
    PRIVATE
        ${DOUBLE_LIB}
)

# -------------------------------------------------------------------------------------------------
# BVH lib
set(BVH_LIB bvh_lib)
add_library(${BVH_LIB} INTERFACE)

target_include_directories(${BVH_LIB}
    INTERFACE
        bvh_lib/inc/
)

target_link_libraries(${BVH_LIB}
    INTERFACE
        ${GEO_LIB}
        ${DOUBLE_LIB}
)

target_compile_features(${BVH_LIB} INTERFACE ${CXX_STANDARD})

