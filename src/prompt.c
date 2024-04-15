/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:22:50 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/15 14:13:43 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
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

bool	isemptycmd(char *s)
{
	int	i;

	if (ft_strlen(s) == 1 &&  s[0] == ':')
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

void	kill_children(t_minishell *msh)
{
	size_t	i;

	i = 0;
	if (g_signum != -1)
	{
		while (i < ft_vector_size(msh->child_pids))
		{
			kill(msh->child_pids[i], g_signum);
			i++;
		}
		g_signum = -1;
	}
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
	printf("Line after expansion `%s`\n", line2);
	if (isemptycmd(line2))
		return (free(line), free(line2), 0);
	else if (ft_strlen(line2) == 1 && line[0] == '!') // TODO: Remove this maybe ?
	{
		msh->exit_code = 2;
		return (0);
	}
	node = parse_line(msh, line2);
	add_our_history(msh, line);
	free(line2);
	free(line);
	if (!node)
	{
		msh_error("parsing error");
		msh->exit_code = 2;
		return (-1);
	}
	dump_line(node);
	msh->exit_code = exec_cmd(msh, node, -1, -1);
	if (msh->exit_code != 0)
		return (-1);
	while (wait(&msh->exit_code) > 0) 
		kill_children(msh);
	msh->exit_code = WEXITSTATUS(msh->exit_code);
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
	ft_fprintf(2, "exit\n");
}
