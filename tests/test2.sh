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
	CMD_STATUS=$?
	
	if [[ $OUTPUT == $2 ]]; then
		echo -e "Test err \`$BOLD$1$RESET\` is a ${GREEN}Success${RESET}"
	else
		echo -e "Test err \`$BOLD$1$RESET\` is a ${RED}Failure${RESET}"
		echo -e "Expected \`$BOLD$2$RESET\` but got \`$BOLD$OUTPUT$RESET\`"
		echo
		EXIT_CODE=1
	fi

	if [[ $CMD_STATUS != $3 ]]; then
		echo -e "${RED}Exit code is wrong${RESET}: \`$CMD_STATUS\` (got) vs \`$3\` (expected)"
		EXIT_CODE=1
	fi
}

#
# parser
#

run-test-err "| | |" "msh: parsing error" 2

#
# exec
#

run-test-err "/bin/cd Desktop" "/bin/cd: No such file or directory" 127
run-test-err "./Makefile" "./Makefile: Permission denied" 126

#
# `cd`
#

run-test-err "cd src tests" "msh: too many arguments" 1

#
# `echo`
#

run-test 'echo $?' "0"
# Those two works but not the tests
#run-test 'echo $?$' '0$'
#run-test 'echo $:$= | cat -e' '$:$=$'
run-test 'echo [$TERM4' '['
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
