/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:31:32 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/17 20:18:06 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static t_tok	next_token(char *line, size_t *index)
{
	t_str	s;
	size_t	i;

	i = *index;
	if (i >= ft_strlen(line))
		return (nulltok());

	while (line[i] && isspace(line[i]))
		i++;

	*index = i;
	if (line[i] == '\0')
		return (nulltok());

	s = str("");

	if (ft_strlen(&line[i]) >= 2
		&& (!ft_strncmp(&line[i], "<<", 2) || !ft_strncmp(&line[i], ">>", 2)
			|| !ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2)))
	{
		str_append_n(&s, &line[i], 2);
		*index = i + 2;
		return (tok(TOK_OP, s.data));
	}
	else if (ft_strlen(&line[i]) >= 1
		&& (line[i] == '<' || line[i] == '>' || line[i] == '|'
			|| line[i] == '(' || line[i] == ')'))
	{
		str_append_n(&s, &line[i], 1);
		*index = i + 1;
		return (tok(TOK_OP, s.data));
	}

	while (i < ft_strlen(line) && !isspace(line[i]) && line[i] != '<'
		&& line[i] != '>' && line[i] != '|' && line[i] != '&' && line[i] != '('
		&& line[i] != ')')
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			*index = i;
			i++;
			while (line[i])
			{
				if ((line[i] == '"' && line[*index] == '"') || (line[i] == '\'' && line[*index] == '\''))
					break ;
				if (line[i] == '\\' && (line[i + 1] == '"' || line[i + 1] == '\''))
				{
					str_append_n(&s, &line[i + 1], 1);
					i++;
				}
				else
					str_append_n(&s, &line[i], 1);
				i++;
			}
		}
		else if (line[i] == '\\' && line[i + 1] == ' ')
		{
			str_append(&s, " ");
			i++;
		}
		else
		{
			str_append_n(&s, &line[i], 1);
		}
		i++;
	}

	*index = i;
	return (tok(TOK_STR, s.data));
}

t_tok	*split_into_tokens(char *line)
{
	size_t	index;
	t_tok	*tokens;
	t_tok	tok;

	tokens = ft_vector(sizeof(t_tok), 0);
	if (!tokens)
		return (NULL);
	index = 0;
	while (1)
	{
		tok = next_token(line, &index);
		if (!tok.s)
			break ;
		if(!ft_vector_add(&tokens, &tok))
			return (ft_vector_deep_free(tokens), NULL);
	}
	return (tokens);
}
