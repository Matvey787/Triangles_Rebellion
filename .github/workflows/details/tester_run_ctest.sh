source "$(dirname "$0")/../project_setup.sh"

function run_tester_ctest() {
    local tester_name="$1"
    declare -n tester_ref="$tester_name"

    echo "Handling ${tester_ref[NAME]} (ctest)"

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

    echo "Running ctest"
    if ctest --test-dir build -V --output-on-failure; then
        echo "✅ CTest passed for ${tester_ref[NAME]}"
        local CTEST_EXIT_CODE=0
    else
        local CTEST_EXIT_CODE=$?
        echo "❌ CTest failed for ${tester_ref[NAME]} with exit code: $CTEST_EXIT_CODE"
    fi

    rm -rf build
    cd "$original_dir" || return 1
    
    return $CTEST_EXIT_CODE
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
        echo "=== Processing ${tester_ref[NAME]} (CTest) ==="
        
        run_tester_ctest "$tester"
        local exit_code=$?
        
        if [[ $exit_code -eq 0 ]]; then
            echo "✅ CTest completed successfully in ${tester_ref[NAME]}."
        else
            echo "❌ CTest failed in ${tester_ref[NAME]} with exit code: $exit_code."
            overall_result=1
        fi
        echo ""
    else
        echo "$tester is missing, skipping..."
        echo ""
    fi
done

exit $overall_result