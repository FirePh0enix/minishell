/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/29 12:01:20 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void __attribute__((noreturn))	exit_n_free(t_minishell *msh, int exit_code)
{
	ft_vector_free(msh->child_pids);
	ft_vector_free(msh->open_fds);
	ft_vector_deep_free(msh->env);
	exit(exit_code);
}

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
		exit_n_free(msh, 0);
	else if (ft_strlen(line) == 1 && line[0] == '!')
		exit_n_free(msh, 2);
	node = parse_line(msh, line);
	free(line);
	if (node == NULL)
		return (msh_error("parsing error"), exit(2), 2);
	else if (node == (void *)1)
		exit_n_free(msh, 1);
	status = exec_cmd(msh, node, -1, -1);
	if (status != 0)
		exit_n_free(msh, status);
	status = wait_for_children(msh);
	free_node(node);
	exit_n_free(msh, status);
}

static void	init_env_else(t_minishell *msh)
{
	int		lvl;
	char	*s2;

	s2 = getourenv(msh, "SHLVL");
	if (!s2)
		setourenv(msh, "SHLVL", "1");
	else
	{
		lvl = ft_atoi(s2) + 1;
		free(s2);
		s2 = ft_itoa(lvl);
		setourenv(msh, "SHLVL", s2);
		free(s2);
	}
}

static void	init_env_if_i(t_minishell *msh)
{
	char	*s2;

	s2 = getourenv(msh, "PATH");
	if (!s2)
		msh->no_env = true;
	else
		free(s2);
	s2 = getourenv(msh, "TERM");
	if (!s2)
		msh->no_color = true;
	else
		free(s2);
	setourenv2(msh, "OLDPWD", "");
	s2 = getcwd(NULL, 0);
	setourenv2(msh, "PWD", s2);
	free(s2);
	setourenv2(msh, "_", "/usr/bin/env");
	init_env_else(msh);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	msh;

	if (!isatty(0) && !TEST)
		return (1);
	ft_bzero(&msh, sizeof(t_minishell));
	msh.exit_code = 0;
	copy_env(&msh, envp);
	load_history(&msh);
	init_env_if_i(&msh);
	init_signals(&msh);
	if (TEST && argc == 2)
		exec_single_cmd(&msh, argv);
	else
		prompt(&msh);
	ft_vector_deep_free(msh.env);
	if (msh.child_pids)
		ft_vector_free(msh.child_pids);
	if (msh.open_fds)
		ft_vector_free(msh.open_fds);
	free_history(&msh);
	rl_clear_history();
	return (msh.exit_code);
}
