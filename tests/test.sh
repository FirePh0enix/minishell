#!/bin/bash

TESTS=(
	simple-command-parsing
	command-parsing-with-pipe
	env-variables
	parenthesis-with-redirections
)

SRCS="tests/lib/assert.c src/parser.c src/wildcard.c src/env.c src/parser/expr.c src/parser/tokenizer.c"

GREEN="\e[42m"
RED="\e[101m"
BOLD="\e[1m"
RESET="\e[0m"

run-test()
{
	clang -o minishell.test tests/$1.c $SRCS libft/libft.a -lreadline -Ilibft -Isrc -g
	OUTPUT=`./minishell.test 2> /dev/null`

	if [ $? -eq 0 ]; then
		echo -e "Test \`$BOLD$1$RESET\` is a ${GREEN}Success${RESET}"
	else
		echo -e "Test \`$BOLD$1\` is a ${RED}Failure${RESET}"
		echo -e "========= Output ========="
		echo "$OUTPUT"
		echo

		if [[ "$OUTPUT" == "" ]]; then
			echo "No output :( The test might has crashed, re-running with valgrind"
			valgrind ./minishell.test
		fi

		return 1
	fi
}

SIGNIFICANT_ERR=0
LAST_ERR=0

for test_name in "${TESTS[@]}"; do
	run-test $test_name
	LAST_ERR=$?
	
	if [[ "$SIGNIFICANT_ERR" == "0" && "$LAST_ERR" != "0" ]]; then
		SIGNIFICANT_ERR=$LAST_ERR
	fi
done

bash tests/test2.sh

LAST_ERR=$?
if [[ $LAST_ERR != 0 ]]; then
	SIGNIFICANT_ERR=$LAST_ERR
fi
exit $SIGNIFICANT_ERR
