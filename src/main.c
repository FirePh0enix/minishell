/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/09 15:25:36 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void	exec_single_cmd(t_minishell *msh, char *argv[])
{
	t_node	*node;

	node = parse_line(msh, argv[1]);
	exec_cmd(msh, node, -1, -1);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	msh;

	(void) argc;
	(void) argv;
	msh.exit_code = 0;
	msh.open_fds = ft_vector(sizeof(int), 0);
	copy_env(&msh, envp);
	init_signals(&msh);
	load_history(&msh);
	if (argc == 1)
		prompt(&msh);
	else if (argc == 2)
		exec_single_cmd(&msh, argv);
	free_history(&msh);
	free_env(&msh);
	rl_clear_history();
	return (msh.exit_code);
}
