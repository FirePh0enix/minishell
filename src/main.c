/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/23 00:51:37 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <stdio.h>

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	minishell;
	t_node		*node;

	minishell.exit_code = 0;
	node = parse_line(&minishell, "ls *");

	copy_env(&minishell, envp);
	init_signals(&minishell);

	printf("%s\n", getourenv(&minishell, "PATH"));

	prompt(&minishell);
	return (minishell.exit_code);
}
