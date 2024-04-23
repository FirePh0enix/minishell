/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:26:05 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/23 15:05:50 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	add_assign(t_minishell *msh, char *s, char *name, int i)
{
	char	*value;
	char	*value2;
	char	*value3;

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

static int	export_variable(t_minishell *msh, char *s, int *exit_code)
{
	size_t	i;
	char	*name;
	char	*value;

	i = 0;
	while (s[i] && s[i] != '=' && s[i] != '+')
		i++;
	name = ft_strndup(s, i);
	if (!is_valid_var_name(name))
		return (set_exit_code(exit_code, 1),
			ft_fprintf(2, "msh: export: `%s' is not a valid identifier\n", s),
			free(name), 1);
	if (!ft_strncmp(s + i, "+=", 2))
		add_assign(msh, s, name, i);
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
	return (free(name), 0);
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

static void	print_vars(t_minishell *msh, int file)
{
	char	**env;
	size_t	i;

	env = ft_vector(sizeof(char *), ft_vector_size(msh->env) - 1);
	i = 0;
	while (i < ft_vector_size(msh->env) - 1)
	{
		ft_vector_add(&env, &msh->env[i]);
		i++;
	}
	sort_str_tab(env, ft_vector_size(env));
	i = 0;
	while (i < ft_vector_size(env))
	{
		print_var(file, env[i]);
		i++;
	}
	ft_vector_free(env);
}

int	builtin_export(t_minishell *msh, int in, int out, t_node *node)
{
	int		file;
	int		i;
	int		exit_code;

	exit_code = 0;
	file = 1;
	if (out != -1)
		file = out;
	if (node->cmd.outfile)
		file = open_outfile(node);
	if (node->cmd.argc == 1)
		return (print_vars(msh, file), exit_code);
	if (in != -1 || out != -1)
		return (exit_code);
	i = 0;
	while (++i < node->cmd.argc)
		export_variable(msh, node->cmd.argv[i], &exit_code);
	return (exit_code);
}
