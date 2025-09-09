#!/bin/bash

TEST_FILE="tests.sh"

while IFS= read -r line || [[ -n "$line" ]]; do
	clear
	echo ""
    echo ">>> $line <<<"
	echo ""
    echo "--- Minishell ---"
	echo ""
    echo "$line" | ./minishell
	echo ""
	echo ""
    echo "--- Bash ---"
	echo ""
    echo "$line" | bash
	echo ""
    echo "Press Enter to continue..."
    read -r -p "" </dev/tty
done < "$TEST_FILE"
