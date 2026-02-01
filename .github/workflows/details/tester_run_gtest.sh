source "$(dirname "$0")/../project_setup.sh"

function run_tester_gtest() {
    local tester_name="$1"
    declare -n tester_ref="$tester_name"

    echo "Handling ${tester_ref[NAME]}"

    cd "${tester_ref[PATH]}"
    echo "Trying to generate a build system"
    cmake -G=Ninja -S . -B "build" -DCMAKE_CXX_COMPILER=clang++ ${tester_ref[CMAKE_OPTIONS]}
    cd -

    echo "Building gtest"
    cmake --build "${tester_ref[PATH]}/build"

    echo "Running gtests"
    IFS=' ' read -ra EXE_FILES <<< "${tester_ref[GTEST_EXE_FILE_NAMES]}"

    for exe_file in "${EXE_FILES[@]}"; do
        TEST_EXE="./${tester_ref[PATH]}/build/${exe_file}"
        
        echo "Executing: $TEST_EXE"
        "$TEST_EXE" --gtest_output=xml:"${tester_ref[PATH]}/build/${exe_file}-results.xml"
        GTEST_EXIT_CODE=$?
        
        if [[ $GTEST_EXIT_CODE -ne 0 ]]; then
            echo "❌ GTest failed with exit code: $TEST_EXIT_CODE"
            rm -rf build
            cd -
            exit 1
        fi
    done
    echo "✅ GTest stage has completed successfully."
}

function tester_exists() {
    local var_name="$1"
    if declare -p "$var_name" 2>/dev/null | grep -q '^declare -A'; then
        return 0
    else
        return 1
    fi
}

ALL_TESTERS=("tester1" "tester2" "tester3" "tester4" "tester5")

for tester in "${ALL_TESTERS[@]}"; do
    if tester_exists "$tester"; then
        run_tester_gtest "$tester"
    else
        echo "$tester is missing, gtesting is complete."
        break
    fi
done