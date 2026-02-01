source "$(dirname "$0")/../project_setup.sh"

function create_exe_for_ctest() {
    local tester_name="$1"
    declare -n tester_ref="$tester_name"

    echo "Create exe for ctest in ${tester_ref[NAME]}"

    local original_dir="$(pwd)"
    
    if ! cd "${tester_ref[CTEST_CHECKED_PROJECT]}"; then
        echo "❌ Cannot cd to ${tester_ref[CTEST_CHECKED_PROJECT]}"
        return 1
    fi
    
    echo "Trying to generate a build system"
    cmake -G="${CMAKE_GENERATOR}" -S . -B "build" -DCMAKE_CXX_COMPILER="${CMAKE_COMPILER}" ${tester_ref[CTEST_CHECKED_PROJECT_CMAKE_OPTIONS]}
    local CMAKE_EXIT_CODE=$?

    if [[ $CMAKE_EXIT_CODE -ne 0 ]]; then
        echo "❌ Cmake failed with exit code: $CMAKE_EXIT_CODE"
        rm -rf build
        cd "$original_dir" || return 1
        return $CMAKE_EXIT_CODE
    fi
    
    echo "Building executable"
    if ! cmake --build build; then
        echo "❌ Build failed"
        rm -rf build
        cd "$original_dir" || return 1
        return 1
    fi

    echo "✅ Executable created successfully"
    
    cd "$original_dir" || return 1
    
    return 0
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
        echo "=== Creating exe for ctest of ${tester_ref[NAME]} ==="
        
        create_exe_for_ctest "$tester"
        local exit_code=$?
        
        if [[ $exit_code -eq 0 ]]; then
            echo "✅ Executable created successfully for ${tester_ref[NAME]}."
        else
            echo "❌ Failed to create executable for ${tester_ref[NAME]} with exit code: $exit_code."
            overall_result=1
        fi
        echo ""
    else
        echo "$tester is missing, skipping..."
        echo ""
    fi
done

exit $overall_result