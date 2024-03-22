/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/22 20:04:37 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	minishell;
	t_node		*node;

	node = parse_line("ls");
	minishell.exit_code = 0;
	
	init_signals(&minishell);

	prompt(&minishell);
	return (minishell.exit_code);
}
