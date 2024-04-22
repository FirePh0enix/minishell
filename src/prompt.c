/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:22:50 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/22 15:35:27 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include "exec.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"

int	event(void)
{
	return (0);
}

bool	isemptycmd(char *s)
{
	int	i;

	if (ft_strlen(s) == 1 && s[0] == ':')
		return (true);
	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

static int	check_and_execute(t_minishell *msh, t_node *node)
{
	if (!node)
	{
		msh_error("parsing error");
		msh->exit_code = 2;
		return (-1);
	}
	else if (node == (void *) 1)
	{
		msh->exit_code = 1;
		return (-1);
	}
	set_sigquit_dump();
	msh->exit_code = exec_cmd(msh, node, -1, -1);
	set_sigquit_default();
	free_node(node);
	if (msh->exit_code != 0)
		return (-1);
	return (0);
}

static int	execute_line(t_minishell *msh, char *line)
{
	t_node	*node;
	char	*line2;

	if (msh->open_fds)
		ft_vector_free(msh->open_fds);
	msh->open_fds = ft_vector(sizeof(int), 0);
	if (msh->child_pids)
		ft_vector_free(msh->child_pids);
	msh->child_pids = ft_vector(sizeof(pid_t), 1);
	line2 = expand_str(msh, line).data;
	if (isemptycmd(line2))
		return (free(line), free(line2), 0);
	node = parse_line(msh, line2);
	add_our_history(msh, line);
	free(line2);
	free(line);
	return (check_and_execute(msh, node));
}

void	prompt(t_minishell *msh)
{
	char	*line;
	char	buf[256];

	rl_event_hook = event;
	while (1)
	{
		if (g_signum == SIGINT)
		{
			g_signum = -1;
			msh->exit_code = 130;
		}
		write_prefix(msh, buf);
		line = readline(buf);
		if (line == NULL)
			break ;
		else if (line[0] == '\0')
			continue ;
		execute_line(msh, line);
	}
	ft_fprintf(2, "exit\n");
}
