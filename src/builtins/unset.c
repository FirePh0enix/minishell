/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:24:38 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 15:00:50 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	unset_env(t_minishell *msh, char *name, int *exit_code)
{
	const size_t	size = ft_strlen(name);
	int				i;

	if (!strcmp(name, "_"))
		return (0);
	if (!is_valid_var_name(name))
		return (set_exit_code(exit_code, 1),
			msh_builtin_error("unset", "invalid parameter name"), 1);
	i = 0;
	while (msh->env[i])
	{
		if (ft_strlen(msh->env[i]) <= size)
		{
			i++;
			continue ;
		}
		if (!ft_strncmp(msh->env[i], name, size) && msh->env[i][size] == '=')
		{
			ft_vector_del(&msh->env, i);
			break ;
		}
		i++;
	}
	return (0);
}

int	builtin_unset(t_minishell *msh, int argc, char *argv[], t_node *node)
{
	int		flags;
	int		file;
	int		exit_code;
	int		i;

	if (argc < 2)
		return (msh_builtin_error("unset", "not enough arguments"), 1);
	exit_code = 0;
	flags = O_WRONLY | O_CREAT;
	if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		if (file == -1)
			return (msh_errno("unset"), -1);
		close(file);
	}
	i = 0;
	while (argv[++i])
		unset_env(msh, argv[i], &exit_code);
	return (0);
}
