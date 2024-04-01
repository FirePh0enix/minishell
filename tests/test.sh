#!/bin/bash

GREEN="\e[42m"
RED="\e[101m"
BOLD="\e[1m"
RESET="\e[0m"

clang -o minishell.test tests/$1.c $2 libft/libft.a -lreadline -Ilibft -Isrc -g
OUTPUT=`./minishell.test`

if [ $? -eq 0 ]; then
	echo -e "Test \`$BOLD$1$RESET\` is a ${GREEN}Success${RESET}"
else
	echo -e "Test \`$BOLD$1\` is a ${RED}Failure${RESET}"
	echo -e "========= Output ========="
	echo "$OUTPUT"
	echo

	if [[ "$OUTPUT" -eq "" ]]; then
		echo "No output :( The test might has crashed, re-running with valgrind"
		valgrind ./minishell.test
	fi

	exit 1
fi
