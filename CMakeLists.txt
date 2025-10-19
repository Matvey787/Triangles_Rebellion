# cmake -S . -B build && cmake --build build

# ./build/out
# cmake --build build --target clean
# ctest --test-dir build -V
# g++ -g -O2 -fno-omit-frame-pointer your_program.cpp -o your_program
# -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Запуск Google tests
# ./build/geo_tests

# Запуск Google tests с фильтром
# ./build/geo_tests --gtest_filter="...*"


cmake_minimum_required(VERSION 3.11)
project(LIRS_Cache CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "GTest not found in system — downloading via FetchContent...")

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    )

    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(SOURCE_FILES
    src/main.cpp
    src/line.cpp
    src/triangle.cpp
    src/double.cpp
    src/2dIntersection_Impl.cpp
    src/3dIntersection_Impl.cpp
)

SET(EXEC_FILE_NAME "out")

add_executable(${EXEC_FILE_NAME} ${SOURCE_FILES})

add_executable(geo_tests tests/test_geo.cpp src/line.cpp src/triangle.cpp src/double.cpp src/2dIntersection_Impl.cpp src/3dIntersection_Impl.cpp)
target_link_libraries(geo_tests GTest::gtest GTest::gtest_main)
target_include_directories(geo_tests PRIVATE ${CMAKE_SOURCE_DIR}/inc)

include(CTest)
enable_testing()

function(create_tests_from_pairs)
    file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/tests/test*.dat)

    if(!TEST_FILES)
        message(WARNING "No test files found.")
        return()
    endif()

    foreach(TEST_FILE ${TEST_FILES})

        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        string(REGEX REPLACE "^test" "" TEST_NUMBER ${TEST_NAME})
        
        set(ANS_FILE ${CMAKE_SOURCE_DIR}/tests/ans${TEST_NUMBER}.dat)
        
        if(EXISTS ${ANS_FILE})
            file(READ ${ANS_FILE} EXPECTED_OUTPUT)
            string(STRIP "${EXPECTED_OUTPUT}" EXPECTED_OUTPUT)
            
            add_test(NAME test_${TEST_NUMBER}
                COMMAND bash -c "../build/${EXEC_FILE_NAME} < test${TEST_NUMBER}.dat"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
            )
            
            set_tests_properties(test_${TEST_NUMBER} PROPERTIES
                PASS_REGULAR_EXPRESSION "^${EXPECTED_OUTPUT}([ \t\n\r]|$)"
            )
        else()
            message(WARNING "Answer file not found for test ${TEST_NUMBER}: ${ANS_FILE}")
        endif()
    endforeach()
endfunction()

create_tests_from_pairs()
