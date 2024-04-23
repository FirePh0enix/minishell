/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 12:02:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/23 15:32:14 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parser.h"
#include "libft.h"

void	expand_tilde(t_minishell *msh, t_str *s, size_t *index);
bool	can_expand_tilde(char *line, size_t i);

bool	not_quotes(char *line, size_t i)
{
	return (can_expand_tilde(line, i) || (line[i + 1] != '\0' && line[i] == '$'
			&& (line[i + 1] == '\'' || line[i + 1] == '"'))
		|| (line[i] == '\\' && line[i + 1] != ' '));
}

void	expand_no_any_quotes(t_minishell *msh, t_str *s, char *line,
	size_t *index)
{
	if (can_expand_tilde(line, *index))
		expand_tilde(msh, s, index);
	else if (line[*index + 1] != '\0' && line[*index] == '$'
		&& (line[*index + 1] == '\'' || line[*index + 1] == '"'))
		*index += 1;
	else if (line[*index] == '\\' && line[*index + 1] != ' ')
		*index += 1;
}

bool	can_expand_no_quotes(char *line, size_t i)
{
	return ((line[i + 1] != '\0' && line[i] == '\\'
			&& (line[i + 1] == '\\' || line[i + 1] == '$'))
		|| can_expand_env(line, i) || (line[i] == '$' && line[i + 1] == '$'));
}

void	expand_no_quotes(t_minishell *msh, t_str *s, char *line, size_t *index)
{
	if (can_expand_env(line, *index))
		expand_env(msh, s, line, index);
	else if (line[*index + 1] != '\0' && line[*index] == '\\'
		&& (line[*index + 1] == '\\' || line[*index + 1] == '$'))
	{
		str_append_n(s, &line[*index + 1], 1);
		*index += 2;
	}
	else if (line[*index] == '$' && line[*index + 1] == '$')
	{
		str_append(s, "$$");
		*index += 2;
	}
}

void	handle_quotes(char *line, size_t i, bool *open_quotes,
			bool *open_dquotes);

t_str	expand_str(t_minishell *msh, char *line)
{
	t_str	s;
	size_t	i;
	bool	open_quotes;
	bool	open_dquotes;

	s = str("");
	i = 0;
	open_quotes = false;
	open_dquotes = false;
	while (i < ft_strlen(line))
	{
		handle_quotes(line, i, &open_quotes, &open_dquotes);
		if (!open_quotes && can_expand_no_quotes(line, i))
			expand_no_quotes(msh, &s, line, &i);
		else if (!open_quotes && !open_dquotes && not_quotes(line, i))
			expand_no_any_quotes(msh, &s, line, &i);
		else if ((open_dquotes || open_quotes) && line[i] == '*')
		{
			str_append(&s, "\'\\*\'");
			i++;
		}
		else
			str_append_n(&s, &line[i++], 1);
	}
	return (s);
}
