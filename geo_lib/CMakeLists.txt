cmake_minimum_required(VERSION 3.17)
project(LIRS_Cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20) 
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
add_library(${GEO_LIB} INTERFACE)

target_link_libraries(${GEO_LIB}
    INTERFACE
        ${DOUBLE_LIB}
)

target_include_directories(${GEO_LIB}
    INTERFACE
        geo_lib/Point/inc
        geo_lib/Vector/inc
        geo_lib/Line/inc
        geo_lib/Triangle/inc
        geo_lib/
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

