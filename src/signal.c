/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:31:08 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/22 19:00:11 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>

int	g_signum;

void	signal_handler(int signum, siginfo_t *si, void *p)
{
	(void) si;
	(void) p;
	g_signum = signum;
	printf("\n");
	print_prompt_prefix();
	rl_on_new_line();
}

void	init_signals(t_minishell *minishell)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = (void *) signal_handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}
