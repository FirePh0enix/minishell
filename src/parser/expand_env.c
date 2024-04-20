/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:22:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/20 13:17:23 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"
#include "../minishell.h"

bool	is_just_after_heredoc(char *line, size_t i);

bool	can_expand_env(char *line, size_t i)
{
	return (line[i] == '$'
		&& ((line[i + 1] == '_' || ft_isalnum(line[i + 1]))
			|| line[i + 1] == '?')
		&& !is_just_after_heredoc(line, i));
}

static void	escape_ch(t_str *s, char c)
{
	str_append(s, "\\");
	str_append(s, &c);
}

static void	append_escaped(t_str *s, char *env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		if (env[i] == '"')
			str_append(s, "\\\"");
		else if (env[i] == '\'')
			str_append(s, "\\'");
		else if (ft_strlen(&env[i]) >= 2 && is_dual_op(&env[i]))
		{
			escape_ch(s, env[i]);
			i++;
			escape_ch(s, env[i]);
		}
		else if (env[i] == '|' || env[i] == '>' || env[i] == '<'
			|| env[i] == '*')
			escape_ch(s, env[i]);
		else
			str_append_n(s, &env[i], 1);
		i++;
	}
}

static void	expand_env2(t_minishell *msh, t_str *s, char *line, size_t *index)
{
	t_str	env_name;
	char	*env;
	size_t	i;

	i = *index;
	env_name = str("");
	while (line[i] && (line[i] == '_' || ft_isalnum(line[i])))
		str_append_n(&env_name, &line[i++], 1);
	env = getourenv(msh, env_name.data);
	if (env)
	{
		append_escaped(s, env);
		free(env);
	}
	*index = i;
}

void	expand_env(t_minishell *msh, t_str *s, char *line, size_t *index)
{
	size_t	i;
	char	*env;

	i = *index;
	i++;
	if (ft_isdigit(line[i]) || line[i] == '*')
		i++;
	else if (line[i] == '?')
	{
		env = getourenv(msh, "?");
		str_append(s, env);
		free(env);
		i++;
	}
	else
		expand_env2(msh, s, line, &i);
	*index = i;
}
