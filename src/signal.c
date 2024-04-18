/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:31:08 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/18 17:28:20 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>

int	g_signum;

void	set_ctrlc_default();

void	signal_handler(int signum, siginfo_t *si, void *p)
{
	(void) si;
	(void) p;
	g_signum = signum;
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_done = 1;
	}
}

void	signal_handler_heredoc(int signum, siginfo_t *si, void *p)
{
	g_signum = SIGINT;
	close(0);
}

void	init_signals(t_minishell *msh)
{
	struct sigaction	sa;

	(void) msh;
	set_ctrlc_default();
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = (void *) signal_handler;
	sa.sa_handler = SIG_IGN; // FIXME: Completly ignore the signal, but we still want to handle it
	sigemptyset(&sa.sa_mask);
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_ctrlc_heredoc()
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = (void *) signal_handler_heredoc;
	sigemptyset(&sa.sa_mask);
}

void	set_ctrlc_default()
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = (void *) signal_handler;
	sigemptyset(&sa.sa_mask);
}
