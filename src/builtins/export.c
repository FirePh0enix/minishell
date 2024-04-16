/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:26:05 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/17 00:45:21 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

bool	is_valid_var_name(char *s)
{
	size_t	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (false);
	i++;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	return (s[i] == '\0');
}

static int	set_exit_code(int *exit_code, int exit_code2)
{
	if (*exit_code == 0)
		*exit_code = exit_code2;
	return (0);
}

static int	export_variable(t_minishell *msh, char *s, int *exit_code)
{
	size_t	i;
	char	*name;
	char	*value;
	char	*value2;
	char	*value3;

	i = 0;
	while (s[i] && s[i] != '=' && s[i] != '+')
		i++;
	name = strndup(s, i);
	if (!is_valid_var_name(name))
		return (set_exit_code(exit_code, 1),
			ft_fprintf(2, "msh: export: `%s' is not a valid identifier\n", s),
			free(name), 1);
	if (!ft_strncmp(s + i, "+=", 2))
	{
		value = ft_strdup(s + i + 2);
		value2 = getourenv(msh, name);
		if (!value2)
			setourenv(msh, name, value);
		else
		{
			value3 = ft_strjoin(value2, value);
			free(value2);
			setourenv(msh, name, value3);
			free(value3);
		}
		free(value);
	}
	else if (!ft_strncmp(s + i, "=", 1))
	{
		value = ft_strdup(s + i + 1);
		setourenv(msh, name, value);
		free(value);
	}
	else if (s[i] != '\0')
		return (set_exit_code(exit_code, 1),
			ft_fprintf(2, "msh: export: `%s' is not a valid identifier\n", s),
			free(name), 1);
	free(name);
	return (0);
}

static void	print_var(int file, char *var)
{
	char	*value;
	size_t	name_size;

	if (ft_strlen(var) >= 2 && !ft_strncmp(var, "_=", 2))
		return ;
	value = ft_strchr(var, '=');
	if (*(value + 1) == '\0')
	{
		ft_fprintf(file, "declare -x ");
		write(file, var, ft_strlen(var) - 1);
		ft_fprintf(file, "\n");
		return ;
	}
	name_size = value - var;
	value += 1;
	ft_fprintf(file, "declare -x ");
	write(file, var, name_size);
	ft_fprintf(file, "=");
	ft_fprintf(file, "\"%s\"\n", value);
}

int	builtin_export(t_minishell *msh, int in, int out, t_node *node)
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
	if (node->cmd.argc == 1)
	{
		// TODO: Order in alphabetical order (probably the same for `env')
		i = 0;
		while (msh->env[i])
		{
			print_var(file, msh->env[i]);
			i++;
		}
		return (exit_code);
	}
	if (in != -1 || out != -1)
		return (exit_code);
	i = 0;
	while (++i < node->cmd.argc)
		export_variable(msh, node->cmd.argv[i], &exit_code);
	return (exit_code);
}
