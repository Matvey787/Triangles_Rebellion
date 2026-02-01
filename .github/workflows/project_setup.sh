# Put all project dependencies here
PROJECT_DEPS=(
    cmake \
    g++ \
    clang \
    ninja-build \
    libgtest-dev \
    libgmock-dev
)

declare -A tester1=(
    [NAME]="GEOLib"
    [PATH]="tests/GEOLib/tester"
    [CMAKE_OPTIONS]="-DCTEST=ON -DGTEST=ON"
    [CTEST_EXE_FILE_PATH]="examples/geo_lib"
    [GTEST_EXE_FILE_NAMES]="geo_tests"
)

