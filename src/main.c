/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/22 18:59:54 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	minishell;
	char		*args[] = {"/usr/bin"};
	t_node		node;

	minishell.exit_code = 0;
	
	init_signals(&minishell);

	node.cmd.cmd = "ls";
	node.cmd.argv = args;
	node.cmd.argc = 1;

	prompt(&minishell);
	return (minishell.exit_code);
}
