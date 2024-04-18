/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 12:02:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/18 16:02:04 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parser.h"
#include "libft.h"
#include <ctype.h>

static bool	is_valid_env_ch(char c)
{
	return (c == '_' || ft_isalnum(c));
}

static bool	is_just_after_heredoc(char *line, size_t i)
{
	if (i <= 2)
		return (false);
	i--;
	while (i >= 2)
	{
		if (ft_strlen(&line[i]) >= 2 && (!ft_strncmp(line, "&&", 2)
				|| !ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "||", 2)))
			break ;
		else if (ft_strlen(&line[i]) >= 2 && !ft_strncmp(line, "<<", 2))
			return (true);
		else if (ft_strlen(&line[i]) >= 1 && (line[i] == '|' || line[i] == '<' || line[i] == '>'))
			break ;
		if (line[i] == '"')
		{
			i--;
			while (i >= 2 && line[i] != '"')
				i--;
		}
		else if (line[i] == '\'')
		{
			i--;
			while (i >= 2 && line[i] != '\'')
				i--;
		}
		i--;
	}
	return (false);
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
		else if (ft_strlen(&env[i]) >= 2 && (!strncmp(&env[i], "||", 2)
				|| !strncmp(&env[i], "&&", 2) || !strncmp(&env[i], "<<", 2)
				|| !strncmp(&env[i], ">>", 2)))
		{
			str_append(s, "\\");
			str_append_n(s, &env[i], 1);
			i++;
			str_append(s, "\\");
			str_append_n(s, &env[i], 1);
		}
		else if (env[i] == '|' || env[i] == '>' || env[i] == '<'
			|| env[i] == '*')
		{
			str_append(s, "\\");
			str_append_n(s, &env[i], 1);
		}
		else
			str_append_n(s, &env[i], 1);
		i++;
	}
}

/*
 * Stage 1: Expansion of environment variables, `~`
 */
t_str	expand_str_stage1(t_minishell *msh, char *line)
{
	t_str	s;
	t_str	env_name;
	char	*env;
	size_t	i;
	bool	open_quotes;
	bool	open_dquotes;

	s = str("");
	i = 0;
	open_quotes = false;
	open_dquotes = false;
	while (i < ft_strlen(line))
	{
		if (line[i] == '"' && !open_quotes)
			open_dquotes = !open_dquotes;
		else if (line[i] == '\'' && !open_dquotes)
			open_quotes = !open_quotes;

		if (!open_quotes && line[i + 1] != '\0' && line[i] == '\\'
			&& (line[i + 1] == '\\' || line[i + 1] == '$'))
		{
			str_append_n(&s, &line[i + 1], 1);
			i += 2;
		}
		else if (!open_quotes && !open_dquotes && line[i] == '\\' && line[i + 1] != ' ')
		{
			i++;
		}
		else if (!open_quotes && !open_dquotes
			&& (i == 0 || line[i - 1] == ' ') && ((ft_strlen(&line[i]) >= 2
			&& line[i] == '~' && line[i + 1] == '/')
				|| (ft_strlen(&line[i]) >= 2 && line[i] == '~' && isspace(line[i]))
				|| (ft_strlen(&line[i]) == 1 && line[i] == '~')))
		{
			env = getourenv(msh, "HOME");
			if (env)
			{
				str_append(&s, env);
				free(env);
			}
			else
			{
				str_append(&s, ".");
			}
			i++;
		}
		else if (!open_quotes && line[i] == '$'
			&& (is_valid_env_ch(line[i + 1]) || line[i + 1] == '?')
			&& !is_just_after_heredoc(line, i))
		{
			i++;
			if (ft_isdigit(line[i]) || line[i] == '*')
				i++;
			else if (line[i] == '?')
			{
				env = getourenv(msh, "?");
				str_append(&s, env);
				free(env);
				i++;
			}
			else
			{
				env_name = str("");
				while (line[i] && is_valid_env_ch(line[i]))
					str_append_n(&env_name, &line[i++], 1);
				env = getourenv(msh, env_name.data);
				if (env)
				{
					if (!open_dquotes)
					{
						append_escaped(&s, env);
					}
					else
					{
						append_escaped(&s, env);
					}
					free(env);
				}
			}
		}
		else if (!open_quotes && !open_dquotes && line[i + 1] != '\0'
			&& line[i] == '$' && (line[i + 1] == '\'' || line[i + 1] == '"'))
		{
			i++;
		}
		else if (!open_quotes && line[i] == '$' && line[i + 1] == '$')
		{
			i += 2;
			str_append(&s, "$$");
		}
		else if ((open_dquotes || open_quotes) && line[i] == '*')
		{
			str_append(&s, "\'\\*\'");
			i++;
		}
		else
		{
			str_append_n(&s, &line[i], 1);
			i++;
		}
	}
	return (s);
}

t_str	expand_str(t_minishell *msh, char *line)
{
	t_str	stage1;

	stage1 = expand_str_stage1(msh, line);
	return (stage1);
}

t_tok	*expand_wildcards(t_tok *tokens)
{
	t_tok	*tokens2;
	size_t	i;
	size_t	i2;
	t_tok	*tokens3;
	char	*s;

	tokens2 = ft_vector(sizeof(t_tok), 0);
	if (!tokens2)
		return (NULL);
	i = 0;
	while (i < ft_vector_size(tokens))
	{
		s = ft_strchr(tokens[i].s, '*');
		if (s && ((i > 0 && s[-1] != '\\') || i == 0))
		{
			tokens3 = wildcard(tokens[i].s);
			if (!tokens3)
				return (NULL);
			if (ft_vector_size(tokens3) == 0)
				ft_vector_add(&tokens2, &tokens[i]);
			else
			{
				i2 = 0;
				while (i2 < ft_vector_size(tokens3))
					ft_vector_add(&tokens2, &tokens3[i2++]);
			}
		}
		else
			ft_vector_add(&tokens2, &tokens[i]);
		i++;
	}
	return (tokens2);
}

static t_tok	clean_token(t_tok tk)
{
	t_str	s;
	size_t	i;

	s = str("");
	i = 0;
	while (tk.s[i])
	{
		if (tk.s[i] == '\\' && tk.s[i + 1] == '*')
		{
			str_append(&s, "*");
			i++;
		}
		else
			str_append_n(&s, &tk.s[i], 1);
		i++;
	}
	free(tk.s);
	return (tok(tk.type, s.data));
}

t_tok	*cleanup_tokens(t_tok *tokens)
{
	size_t	i;

	i = 0;
	while (i < ft_vector_size(tokens))
	{
		tokens[i] = clean_token(tokens[i]);
		i++;
	}
	return (tokens);
}
