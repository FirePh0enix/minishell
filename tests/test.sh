#!/bin/bash

GREEN="\e[42m"
RED="\e[101m"
BOLD="\e[1m"
RESET="\e[0m"

EXIT_CODE=0

run-test()
{
	OUTPUT=`./minishell "$1" 2> /dev/null`
	
	if [[ "$OUTPUT" == "$2" ]]; then
		echo -n -e "Test \`$BOLD"
		echo -n $1
		echo -e "$RESET\` is a ${GREEN}Success${RESET}"
	else
		echo -e "Test \`$BOLD$1$RESET\` is a ${RED}Failure${RESET}"
		echo -n -e "Expected \`$BOLD"
		echo -n $2
		echo -e "$RESET\` but got \`$BOLD$OUTPUT$RESET\`"
		echo
		EXIT_CODE=1
	fi
}

run-test-err()
{
	OUTPUT=`./minishell "$1" 2>&1`
	CMD_STATUS=$?
	
	if [[ "$OUTPUT" == "$2" ]]; then
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

run-test-status()
{
	OUTPUT=`./minishell "$1" 2>&1`
	CMD_STATUS=$?

	if [[ "$CMD_STATUS" == "$2" ]]; then
		echo -e "Test err \`$BOLD$1$RESET\` is a ${GREEN}Success${RESET}"
	else
		echo -e "Test err \`$BOLD$1$RESET\` is a ${RED}Failure${RESET}"
		echo -e "Expected \`$BOLD$2$RESET\` but got \`$BOLD$CMD_STATUS$RESET\`"
		echo
		EXIT_CODE=1
	fi
}

#
# parser
#

run-test-err "| | |" "msh: parsing error" 2
run-test-err "echo hola <<<< bonjour" "msh: parsing error" 2
run-test-err "|||||||||||||" "msh: parsing error" 2
run-test-err ">>|><" "msh: parsing error" 2

run-test-err "\"\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r\"" "msh: cannot find command \`e'c'ho 'b'onjo'u'r\`" 127
# Too specific
# run-test-err '?$HOME' "msh: cannot find command \`?/home/phoenixdev\`" 127
run-test-err '$' 'msh: cannot find command `$`' 127

export HOLA="s -la"
run-test-status 'l$HOLA' 0
run-test-err 'l"$HOLA"' "msh: cannot find command \`ls -la\`" 127

#
# exec
#

run-test-err "      " "" 0
run-test-err "!" "" 2

run-test-err "/bin/cd Desktop" "msh: /bin/cd: No such file or directory" 127
run-test-err "./Makefile" "msh: ./Makefile: Permission denied" 126

#
# `cd`
#

run-test-err "cd src tests" "msh: too many arguments" 1

#
# `echo`
#

run-test 'echo $?' "0"
run-test 'echo $?$?' "00"
# Those two works but not the tests
run-test 'echo $?$' '0$'
run-test 'echo $:$= | cat -e' '$:$=$'
run-test 'echo [$TERM4' '['
run-test "echo Hello World" "Hello World"
run-test "echo -----nnnnnn" "-----nnnnnn"
run-test "\"\"''echo hola\"\"'''' que\"\"'' tal\"\"''" "hola que tal"
run-test 'echo $9HOME' "HOME"
run-test 'echo $HOME%' "$HOME%"

run-test "echo \"hola\"" "hola"
run-test "echo 'hola'" "hola"
run-test 'echo "$DONTEXIST""Makefile"' "Makefile"
run-test 'echo "$DONTEXIST""Makefile"' "Makefile"
run-test 'echo "$DONTEXIST" "Makefile"' " Makefile"

run-test 'echo \$HOME' '$HOME'
# Those tests don't run properly but works when doing them manually
run-test 'echo \n' 'n'
#run-test "echo \"\\n\"" '\n'
#run-test 'echo "\\"' '\'
run-test "echo '\\\\'" '\\'

run-test "echo $\"HOME\"" "HOME"
run-test "echo $'Hello'" "Hello"

#
# `pwd`
#

run-test "pwd" "$PWD"
run-test "pwd test" "$PWD"

if [[ "$EXIT_CODE" != "0" ]]; then
	exit 1
fi
