/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:31:08 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/24 14:43:27 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <signal.h>

int	g_signum;

void	set_ctrlc_default(void);
void	set_sigquit_default(void);

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
