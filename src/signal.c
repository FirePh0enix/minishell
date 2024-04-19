/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:31:08 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 11:54:14 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>

int	g_signum;

void	set_ctrlc_default();
void	set_sigquit_default();

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
	(void) signum;
	(void) si;
	(void) p;
	g_signum = SIGINT;
	rl_done = 1;
}

void	sigquit_handler_dump(int signum, siginfo_t *si, void *p)
{
	(void) si;
	(void) p;
	ft_fprintf(2, "Quit (core dumped)\n");
	g_signum = signum;
}

void	init_signals(t_minishell *msh)
{
	(void) msh;
	set_ctrlc_default();
	set_sigquit_default();
}

void	set_ctrlc_heredoc()
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = (void *) signal_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void	set_ctrlc_default()
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = (void *) signal_handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void	set_sigquit_default()
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_sigquit_dump()
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sigquit_handler_dump;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGQUIT, &sa, NULL);
}
