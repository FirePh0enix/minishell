/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:02:43 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/23 17:00:47 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	isnumeric(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && ft_isspace(s[i]))
		i++;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}

static bool	does_overflow_long(char *s)
{
	int			i;
	__int128	n;
	int			sign;

	i = 0;
	while (s[i] && ft_isspace(s[i]))
		i++;
	sign = 1;
	if (s[i] == '-')
	{
		i++;
		sign = -1;
	}
	else if (s[i] == '+')
		i++;
	n = 0;
	while (ft_isdigit(s[i]))
	{
		n = n * 10 + (s[i] - '0');
		if (n * sign < LONG_MIN || n * sign > LONG_MAX)
			return (true);
		i++;
	}
	return (false);
}

static int	exit_shell(t_minishell *msh, char *s, bool fake)
{
	long	i;

	if (fake && (!isnumeric(s) || does_overflow_long(s)))
		return (ft_fprintf(2, EXIT_ERRNUM, s), 2);
	if (!fake && (!isnumeric(s) || does_overflow_long(s)))
		return (ft_fprintf(2, EXIT_ERRNUM, s), msh->end = true, 2);
	i = ft_atoi(s);
	if (!fake)
		msh->end = true;
	return (i);
}

int	builtin_exit(t_minishell *msh, int in, int out, t_node *node)
{
	ft_fprintf(1, "exit\n");
	if ((node->cmd.argc >= 2 && !isnumeric(node->cmd.argv[1]))
		|| (node->cmd.argc == 2))
		return (exit_shell(msh, node->cmd.argv[1], in != -1 || out != -1));
	else if (node->cmd.argc == 1)
		return (exit_shell(msh, "0", in != -1 || out != -1));
	else
		return (msh_builtin_error("exit", "too many arguments"), -1);
}
