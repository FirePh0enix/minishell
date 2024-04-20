/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:32:47 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/20 12:33:35 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quotes(char *line, size_t i, bool *open_quotes,
		bool *open_dquotes)
{
	if (line[i] == '"' && !*open_quotes)
		*open_dquotes = !*open_dquotes;
	else if (line[i] == '\'' && !*open_dquotes)
		*open_quotes = !*open_quotes;
}

void	expand_tilde(t_minishell *msh, t_str *s, size_t *index)
{
	char	*env;

	env = getourenv(msh, "HOME");
	if (env)
	{
		str_append(s, env);
		free(env);
	}
	else
		str_append(s, ".");
	*index += 1;
}

#define ISSP ft_isspace

bool	can_expand_tilde(char *line, size_t i)
{
	return ((i == 0 || line[i - 1] == ' ') && ((ft_strlen(&line[i]) >= 2
				&& line[i] == '~' && line[i + 1] == '/')
			|| (ft_strlen(&line[i]) >= 2 && line[i] == '~' && ISSP(line[i]))
			|| (ft_strlen(&line[i]) == 1 && line[i] == '~')));
}
