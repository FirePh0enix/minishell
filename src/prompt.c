/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:22:50 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/23 00:51:56 by ledelbec         ###   ########.fr       */
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

void	print_prompt_prefix()
{
	char	cwd[64];

	getcwd(cwd, 64);
	printf("%s $ ", cwd);
}

void	prompt(t_minishell *minishell)
{
	char	*line;
	t_node	*node;
	char	cwd[64];
	char	buf[64];

	while (1)
	{
		if (g_signum == SIGINT)
		{
			g_signum = -1;
			minishell->exit_code = 130;
		}
		getcwd(cwd, 64);
		ft_sprintf(buf, "%s $ ", cwd);
		line = readline(buf);
		if (line == NULL)
			break ;
		node = parse_line(minishell, line);
		add_history(line);
		// Do something with the binary tree here !
	}
}
