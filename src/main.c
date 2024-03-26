/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/26 19:06:10 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	minishell;
	minishell.exit_code = 0;
	pipe(minishell.pipe);
	copy_env(&minishell, envp);
	init_signals(&minishell);

	prompt(&minishell);
	return (minishell.exit_code);
}
