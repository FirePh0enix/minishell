/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/15 15:27:49 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

// FIXME: Use the same function for single_cmd / prompt for consistency
int	exec_single_cmd(t_minishell *msh, char *argv[])
{
	t_node	*node;
	int		status;
	char	*line;

	msh->open_fds = ft_vector(sizeof(int), 0);
	msh->child_pids = ft_vector(sizeof(pid_t), 1);
	line = argv[1];
	line = expand_str(msh, line).data;
	if (isemptycmd(line))
		exit(0);
	else if (ft_strlen(line) == 1 && line[0] == '!')
		exit(2);
	node = parse_line(msh, line);
	if (node == NULL)
	{
		msh_error("parsing error");
		exit(2);
	}
	status = exec_cmd(msh, node, -1, -1);
	if (status != 0)
		exit(status);
	status = wait_for_children(msh);
	free_node(node);
	exit(status);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	msh;
	char		*s2;

	(void) argc;
	(void) argv;
	ft_bzero(&msh, sizeof(t_minishell));
	msh.exit_code = 0;
	copy_env(&msh, envp);

	if (ft_vector_size(msh.env) == 1)
	{
		setourenv(&msh, "OLDPWD", "");
		s2 = getcwd(NULL, 0);
		setourenv(&msh, "PWD", s2);
		free(s2);
		setourenv(&msh, "SHLVL", "1");
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
