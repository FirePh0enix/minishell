/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/12 16:53:36 by vopekdas         ###   ########.fr       */
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
	int		status;

	node = parse_line(msh, argv[1]);
	if (node == NULL)
	{
		msh_error("parsing error");
		exit(2);
	}
	status = exec_cmd(msh, node, -1, -1);
	free_node(node);
	exit(status);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	msh;
	char		*s2;

	(void) argc;
	(void) argv;
	msh.exit_code = 0;
	msh.open_fds = ft_vector(sizeof(int), 0);
	copy_env(&msh, envp);

	if (ft_vector_size(msh.env) == 1)
	{
		s2 = getcwd(NULL, 0);
		setourenv(&msh, "PWD", s2);
		free(s2);
		setourenv(&msh, "SHLVL", "1");
		setourenv(&msh, "_", "/usr/bin/env"); // TODO: could be better
	}
	else
	{
		s2 = getourenv(&msh, "SHLVL");
		if (!s2)
			setourenv(&msh, "SHLVL", "1");
		else
		{
			int	lvl = ft_atoi(s2) + 1;
			free(s2);
			s2 = ft_itoa(lvl);
			setourenv(&msh, "SHLVL", s2);
			free(s2);
		}
	}

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
