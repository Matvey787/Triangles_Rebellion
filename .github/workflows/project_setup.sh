# Put all project dependencies here

PROJECT_DEPS=(
    cmake \
    g++ \
    clang \
    ninja-build \
    libgtest-dev \
    libgmock-dev
)



# Here you can specify the generator and C++ compiler needed for testers.

CMAKE_GENERATOR="Ninja"
CMAKE_COMPILER="clang++"



# Here is a list of all testers who will be available. 
# It is not a problem if the tester's name is present but their declarations (described below) are 
# not - such testers will be skipped.

ALL_TESTERS=("tester1" "tester2" "tester3" "tester4" "tester5")



# Tester declaration 

# !!!ATTENTION!!! All of the following parameters must be filled in. It is assumed that the tester 
# is assembled and compiled relative to the tester folder.

# If the parameter says (support listing), you can specify a list in a string. Listing in a single 
# line separated by spaces.

#   [NAME] - tester name
#   [PATH] - relative path to the repository root of the tester
#   [CMAKE_OPTIONS] - special flags for cmake of tester (support listing)
#   [GTEST_EXE_FILES_NAMES] - all gtest exe files (support listing)
#   [CTEST_CHECKED_PROJECT] - the path to the project folder relative to the repository root, 
#                             compiling the exe testable by ctest. It is assumed that this folder 
#                             contains CMakeLists.txt so that it can be compiled
#   [CTEST_CHECKED_PROJECT_CMAKE_OPTIONS] - special flags for cmake in project testable by ctest 
#                                           (support listing)

declare -A tester1=(
    [NAME]="GEOLib"
    [PATH]="tests/GEOLib/tester/"
    [CMAKE_OPTIONS]="-DCTEST=ON -DGTEST=ON"
    [GTEST_EXE_FILE_NAMES]="geo_tests"
    [CTEST_CHECKED_PROJECT]="examples/geo_lib/"
    [CTEST_CHECKED_PROJECT_CMAKE_OPTIONS]=""
)

