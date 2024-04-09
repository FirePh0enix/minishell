#!/bin/bash

GREEN="\e[42m"
RED="\e[101m"
BOLD="\e[1m"
RESET="\e[0m"

EXIT_CODE=0

run-test()
{
	OUTPUT=`./minishell "$1" 2> /dev/null`
	
	if [[ $OUTPUT == $2 ]]; then
		echo -e "Test \`$BOLD$1$RESET\` is a ${GREEN}Success${RESET}"
	else
		echo -e "Test \`$BOLD$1$RESET\` is a ${RED}Failure${RESET}"
		echo -e "Expected \`$BOLD$2$RESET\` but got \`$BOLD$OUTPUT$RESET\`"
		echo
		EXIT_CODE=1
	fi
}

#
# `echo`
#

run-test "echo -----nnnnnn" "-----nnnnnn"
run-test "\"\"''echo hola\"\"'''' que\"\"'' tal\"\"''" "hola que tal"
# run-test 'echo $9HOME' "HOME"

if [[ "$EXIT_CODE" != "0" ]]; then
	exit 1
fi
