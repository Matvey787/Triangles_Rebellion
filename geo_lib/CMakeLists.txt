cmake_minimum_required(VERSION 3.17)
project(LIRS_Cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD "cxx_std_20")

# -------------------------------------------------------------------------------------------------
# Double lib
set(DOUBLE_LIB double_lib)
add_library(${DOUBLE_LIB} INTERFACE)



target_include_directories(${DOUBLE_LIB}
    INTERFACE
        inc/
)

target_compile_features(${DOUBLE_LIB} INTERFACE ${CXX_STANDARD})


# -------------------------------------------------------------------------------------------------
# Point lib
set(POINT_LIB point_lib)
add_library(${POINT_LIB} INTERFACE)

target_include_directories(${POINT_LIB}
    INTERFACE
        inc/
)

target_link_libraries(${POINT_LIB}
    INTERFACE
        ${DOUBLE_LIB}
)

target_compile_features(${POINT_LIB} INTERFACE ${CXX_STANDARD})


# -------------------------------------------------------------------------------------------------
# Vector lib
set(VECTOR_LIB vector_lib)
add_library(${VECTOR_LIB} INTERFACE)

target_include_directories(${VECTOR_LIB}
    INTERFACE
        inc/
)

target_link_libraries(${VECTOR_LIB}
    INTERFACE
        ${POINT_LIB}
        ${DOUBLE_LIB}
)

target_compile_features(${VECTOR_LIB} INTERFACE ${CXX_STANDARD})


# -------------------------------------------------------------------------------------------------
# Line lib
set(LINE_LIB line_lib)
add_library(${LINE_LIB} INTERFACE)

target_include_directories(${LINE_LIB}
    INTERFACE
        inc/
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
set(TRIANGLE_LIB triangle_lib)
add_library(${TRIANGLE_LIB} INTERFACE)

target_include_directories(${TRIANGLE_LIB}
    INTERFACE
        inc/
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
# Geo (point, vector, line, triangle libs)
set(GEO_LIB geo_lib)
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
set(BVH_LIB bvh_lib)
add_library(${BVH_LIB} INTERFACE)

target_include_directories(${BVH_LIB}
    INTERFACE
        inc/
)

target_link_libraries(${BVH_LIB}
    INTERFACE
        ${GEO_LIB}
        ${DOUBLE_LIB}
)

target_compile_features(${BVH_LIB} INTERFACE ${CXX_STANDARD})

