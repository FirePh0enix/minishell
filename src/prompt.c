/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:22:50 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/09 15:19:26 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include "libft.h"
#include "minishell.h"
#include "parser.h"

void	write_prefix(t_minishell *msh, char buf[])
{
	char	cwd[256];
	char	*home;
	size_t	sz;
	size_t	cwd_sz;

	getcwd(cwd, 256);
	home = getourenv(msh, "HOME");
	if (!home)
	{
		ft_sprintf(buf, COL_LGREEN " %s " COL_LBLUE "$ " COL_RESET, cwd);
		return ;
	}
	sz = ft_strlen(home);
	cwd_sz = ft_strlen(cwd);
	if (cwd_sz >= sz && !ft_strncmp(home, cwd, sz))
		ft_sprintf(buf, COL_LGREEN " ~%s " COL_LBLUE "$ " COL_RESET, cwd + sz);
	else
		ft_sprintf(buf, COL_LGREEN " %s " COL_LBLUE "$ " COL_RESET, cwd);
	free(home);
}

int	event(void)
{
	return (0);
}

static int	execute_line(t_minishell *msh, char *line)
{
	t_node	*node;

	node = parse_line(msh, line);
	add_our_history(msh, line);
	free(line);
	if (!node)
	{
		msh_error("parsing error");
		return (-1);
	}
	dump_line(node);
	msh->exit_code = exec_cmd(msh, node, -1, -1);
	free_node(node);
	return (0);
}

void	prompt(t_minishell *msh)
{
	char	*line;
	char	buf[256];

	rl_event_hook = event;
	while (1)
	{
		ft_vector_clear(msh->open_fds);
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
		if (execute_line(msh, line) == -1)
			continue ;
	}
}
