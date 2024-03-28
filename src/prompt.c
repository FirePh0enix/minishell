/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:22:50 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/28 13:37:43 by ledelbec         ###   ########.fr       */
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
	size_t	home_sz;
	size_t	cwd_sz;

	getcwd(cwd, 256);
	home = getourenv(msh, "HOME");
	if (!home)
	{
		ft_sprintf(buf, COL_LGREEN " %s $ " COL_RESET, cwd);
		return ;
	}
	home_sz = ft_strlen(home);
	cwd_sz = ft_strlen(cwd);
	if (cwd_sz >= home_sz && !ft_strncmp(home, cwd, home_sz))
		ft_sprintf(buf, COL_LGREEN " ~%s " COL_LBLUE "$ " COL_RESET, cwd + home_sz);
	else
		ft_sprintf(buf, COL_LGREEN " %s " COL_LBLUE "$" COL_RESET, cwd);
}

void	prompt(t_minishell *msh)
{
	char	*line;
	t_node	*node;
	char	buf[256];

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
		node = parse_line(msh, line);
		if (!node)
		{
			msh_error("parsing error");
			continue ;
		}
		dump_line(node);
		add_our_history(msh, line);
		msh->exit_code = exec_cmd(msh, node, -1, -1);
	}
}
