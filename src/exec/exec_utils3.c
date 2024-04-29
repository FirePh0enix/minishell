/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:21:08 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/29 12:02:26 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	code_for_cmd(char *cmd)
{
	if (cmd == (void *) 1)
		return (126);
	else if (cmd == (void *) 2)
		return (127);
	return (0);
}

void	add_env(t_minishell *msh, t_node *node)
{
	size_t	i;
	char	*s;
	char	*env;
	char	*name;
	char	*value;

	i = 0;
	while (i < ft_vector_size(node->cmd.env))
	{
		env = node->cmd.env[i];
		s = ft_strchr(env, '=');
		name = ft_strndup(env, s - env);
		value = ft_strdup(s + 1);
		if (ft_strcmp(name, "_"))
			setourenv(msh, name, value);
		free(name);
		free(value);
		i++;
	}
}
