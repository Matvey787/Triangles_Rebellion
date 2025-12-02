# Tester framework configuration

# CTest
set(CTEST_PROGRAM_EXEC   "${CMAKE_SOURCE_DIR}/build/out")
set(CTEST_INPUTS_DIR     "${CMAKE_SOURCE_DIR}/libs/Geo_Lib/Triangle/tests/e2e/dat")
set(CTEST_ANSWERS_DIR    "${CMAKE_SOURCE_DIR}/libs/Geo_Lib/Triangle/tests/e2e/ans")

set(CTEST_INPUT_PATTERN  "dat")
set(CTEST_ANSWER_PATTERN "ans")
set(CTEST_PREFIX         "...")
set(CTEST_TIMEOUT        30)

# Gtest
set(GTEST_DIR "${CMAKE_SOURCE_DIR}/libs/Geo_Lib/Triangle/tests/gtest")
set(GTEST_OUTPUT_EXEC_FILE_NAME "geo_tests")
set(GTEST_USERDEFINED_ADDITIONAL_LIBS "bvh" "geo")










# Do not change without any special reason
set(CTESTER_SCRIPT       "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tester_script.py")
set(GTEST_FETCH_URL "https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip")

string(REPLACE "${CMAKE_SOURCE_DIR}" "" TESTER_RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")