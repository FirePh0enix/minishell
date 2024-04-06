/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/06 15:52:08 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	msh;

	(void) argc;
	(void) argv;
	msh.exit_code = 0;
	msh.open_fds = ft_vector(sizeof(int), 0);
	copy_env(&msh, envp);
	init_signals(&msh);
	load_history(&msh);
	prompt(&msh);
	free_history(&msh);
	free_env(&msh);
	rl_clear_history();
	return (msh.exit_code);
}
