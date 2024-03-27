/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:41:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/28 00:18:52 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	msh;

	msh.exit_code = 0;
	pipe(msh.pipe);
	copy_env(&msh, envp);
	init_signals(&msh);
	/* add_history("cat Makefile | grep all");
	add_history("cat Makefile | < compile_flags.txt grep all");
	add_history("< compile_flags.txt grep all");
	add_history("cat Makefile > test.txt");
	add_history("cat Makefile > test.txt | grep all"); */
	load_history(&msh);
	prompt(&msh);
	return (msh.exit_code);
}
