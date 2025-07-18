#!/bin/bash

TEST_DIR="test"
BUILD_DIR="build/test_objects"
BIN_DIR="build"
FMTPRINT_SRC="src/*.c"
INCLUDE_DIR="include"

mkdir -p "$BUILD_DIR"

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

PASS=0
FAIL=0

for file in "$TEST_DIR"/test_*.c; do 
	testname=$(basename "$file" .c)
	bin="$BIN_DIR/$testname"

	# Compile
	gcc -std=c89  -I"$INCLUDE_DIR" -o "$bin" "$file" $FMTPRINT_SRC
	if [[ $? -ne 0 ]]; then
		echo -e "${RED}Compile failed: $testname${RESET}"
		((FAIL++))
		continue
	fi

	# Run and capture output
	output=$("$bin")
	expected_file="${file%.c}.out"

	if [[ -f "$expected_file" ]]; then
		expected=$(<"$expected_file")
		if [[ "$output" == "$expected" ]]; then
			echo -e "${GREEN}PASS${RESET}"
			((PASS++))
		else
			echo -e "${RED}FAIL${RESET} $testname"
			echo "Expected:"
			echo "$expected"
			echo "Got:"
			echo "$output"
			((FAIL++))
		fi
	else
		echo -e "${YELLOW}WARN{$RESET} $testname (no .out file, skipping check)"
	fi
done
