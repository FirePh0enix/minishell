/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:26:05 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/15 14:43:31 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_exit_code(int *exit_code, int exit_code2)
{
	if (*exit_code == 0)
		*exit_code = exit_code2;
	return (0);
}

static int	export_variable(t_minishell *msh, char *arg, int *exit_code)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (setourenv(msh, arg, ""), 0);
	key = strndup(arg, eq - arg);
	value = ft_strdup(eq + 1);
	if (key[0] == '\0')
		return (free(key), free(value),
			msh_builtin_error("export", "missing key"),
			set_exit_code(exit_code, 1));
	if (ft_strlen(key) >= 1 && !ft_isalpha(key[0])) // FIXME: Check identifiers better
		return (free(key), free(value),
			msh_builtin_error("export", "invalid identifier"),
			set_exit_code(exit_code, 1));
	setourenv(msh, key, value);
	free(key);
	free(value);
	return (0);
}

int	builtin_export(t_minishell *msh, int ac, char *av[], int in, int out, t_node *node)
{
	int		flags;
	int		file;
	int		i;
	int		exit_code;

	exit_code = 0;
	flags = O_WRONLY | O_CREAT;
	file = 1;
	if (out != -1)
		file = out;
	if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		close(file);
	}
	if (ac == 1)
	{
		// FIXME:
		// TODO:
		i = 0;
		while (msh->env[i])
		{
			if (*(ft_strchr(msh->env[i], '=') + 1) != '\0')
				ft_fprintf(file, "declare -x %s\n", msh->env[i]);
			i++;
		}
		return (exit_code);
	}
	if (in != -1 || out != -1)
		return (exit_code);
	i = 0;
	while (++i < ac)
		export_variable(msh, av[i], &exit_code);
	return (exit_code);
}
