source "$(dirname "$0")/../project_setup.sh"

function run_tester_gtest() {
    local tester_name="$1"
    declare -n tester_ref="$tester_name"

    echo "Handling ${tester_ref[NAME]} (gtest)"

    local original_dir="$(pwd)"
    
    if ! cd "${tester_ref[PATH]}"; then
        echo "❌ Cannot cd to ${tester_ref[PATH]}"
        return 1
    fi
    
    echo "Trying to generate a build system"
    cmake -G="${CMAKE_GENERATOR}" -S . -B "build" -DCMAKE_CXX_COMPILER="${CMAKE_COMPILER}" ${tester_ref[CMAKE_OPTIONS]}
    local CMAKE_EXIT_CODE=$?

    if [[ $CMAKE_EXIT_CODE -ne 0 ]]; then
        echo "❌ Cmake failed with exit code: $CMAKE_EXIT_CODE"
        rm -rf build
        cd "$original_dir" || return 1
        return $CMAKE_EXIT_CODE
    fi

    echo "Building gtest"
    cmake --build build
    local BUILD_EXIT_CODE=$?

    if [[ $BUILD_EXIT_CODE -ne 0 ]]; then
        echo "❌ Build failed with exit code: $BUILD_EXIT_CODE"
        rm -rf build
        cd "$original_dir" || return 1
        return $BUILD_EXIT_CODE
    fi

    echo "Running gtests"
    IFS=' '
    read -ra EXE_FILES <<< "${tester_ref[GTEST_EXE_FILE_NAMES]}"
    
    local overall_exit_code=0

    for exe_file in "${EXE_FILES[@]}"; do
        TEST_EXE="./build/${exe_file}"
        
        echo "Executing: $TEST_EXE"
        if "$TEST_EXE" --gtest_output=xml:"build/${exe_file}-results.xml"; then
            echo "✅ $exe_file passed"
        else
            local GTEST_EXIT_CODE=$?
            echo "❌ $exe_file failed with exit code: $GTEST_EXIT_CODE"
            overall_exit_code=$GTEST_EXIT_CODE
        fi
    done
    
    rm -rf build
    cd "$original_dir" || return 1
    
    return $overall_exit_code
}

function tester_exists() {
    local var_name="$1"
    if declare -p "$var_name" 2>/dev/null | grep -q '^declare -A'; then
        return 0
    else
        return 1
    fi
}

overall_result=0

for tester in "${ALL_TESTERS[@]}"; do
    if tester_exists "$tester"; then
        declare -n tester_ref="$tester"
        echo "=== Processing ${tester_ref[NAME]} ==="
        
        run_tester_gtest "$tester"
        local exit_code=$?
        
        if [[ $exit_code -eq 0 ]]; then
            echo "✅ GTests completed successfully in ${tester_ref[NAME]}."
        else
            echo "❌ GTest failed in ${tester_ref[NAME]} with exit code: $exit_code."
            overall_result=1
        fi
        echo ""
    else
        echo "$tester is missing, skipping..."
        echo ""
    fi
done

exit $overall_result