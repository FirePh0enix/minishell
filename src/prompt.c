/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:22:50 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/22 20:04:08 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
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

	while (1)
	{
		if (g_signum == SIGINT)
		{
			g_signum = -1;;
			minishell->exit_code = 130;
		}
		print_prompt_prefix();
		line = readline(NULL);
		if (line == NULL || line[0] == '\0')
			break ;
		node = parse_line(line);
		// Do something with the binary tree here !
	}
}
