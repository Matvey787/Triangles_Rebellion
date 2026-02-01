source "$(dirname "$0")/../project_setup.sh"

echo "# Test Summary" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "**✅ Testing completed successfully**" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "**Build Info:**" >> $GITHUB_STEP_SUMMARY
echo "- Generator: ${CMAKE_GENERATOR}" >> $GITHUB_STEP_SUMMARY
echo "- Compiler: ${CMAKE_COMPILER}" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY

function tester_exists() {
    local var_name="$1"
    if declare -p "$var_name" 2>/dev/null | grep -q '^declare -A'; then
        return 0
    else
        return 1
    fi
}

echo "**Tasters launched:**" >> "$GITHUB_STEP_SUMMARY"

for i in "${!ALL_TESTERS[@]}"; do
    tester="${ALL_TESTERS[i]}"
    if tester_exists "$tester"; then
        declare -n tester_ref="$tester"
        echo "$((i+1)). [${tester_ref[NAME]}](${tester_ref[PATH]})" >> "$GITHUB_STEP_SUMMARY"
    fi
done
echo "" >> $GITHUB_STEP_SUMMARY
echo "Finished at $(date)" >> $GITHUB_STEP_SUMMARY