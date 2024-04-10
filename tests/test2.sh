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

run-test-err()
{
	OUTPUT=`./minishell "$1" 2>&1`
	
	if [[ $OUTPUT == $2 ]]; then
		echo -e "Test err \`$BOLD$1$RESET\` is a ${GREEN}Success${RESET}"
	else
		echo -e "Test err \`$BOLD$1$RESET\` is a ${RED}Failure${RESET}"
		echo -e "Expected \`$BOLD$2$RESET\` but got \`$BOLD$OUTPUT$RESET\`"
		echo
		EXIT_CODE=1
	fi
}

#
# parser tests
#

run-test-err "| | |" "msh: parsing error"

#
# `echo`
#

run-test 'echo $?' "0"
run-test "echo Hello World" "Hello World"
run-test "echo -----nnnnnn" "-----nnnnnn"
run-test "\"\"''echo hola\"\"'''' que\"\"'' tal\"\"''" "hola que tal"
run-test 'echo $9HOME' "HOME"
run-test 'echo $HOME%' "$HOME%"

#
# `pwd`
#

run-test "pwd" "$PWD"
run-test "pwd test" "$PWD"

if [[ "$EXIT_CODE" != "0" ]]; then
	exit 1
fi
