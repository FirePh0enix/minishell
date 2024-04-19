/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:31:32 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 14:15:30 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static bool	is_single_op(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '(' || c == ')');
}

static bool	is_dual_op(char *line)
{
	return (!ft_strncmp(line, "<<", 2) || !ft_strncmp(line, ">>", 2)
		|| !ft_strncmp(line, "&&", 2) || !ft_strncmp(line, "||", 2));
}

static t_tok	next_token(char *line, size_t *index)
{
	t_str		s;
	size_t		i;
	t_tok_type	tt;

	i = *index;
	if (i >= ft_strlen(line))
		return (nulltok());
	while (line[i] && isspace(line[i]))
		i++;
	*index = i;
	if (line[i] == '\0')
		return (nulltok());
	s = str("");
	tt = TOK_IDENT;
	if (ft_strlen(&line[i]) >= 2 && is_dual_op(&line[i]))
	{
		str_append_n(&s, &line[i], 2);
		*index = i + 2;
		return (tok(TOK_OP, s.data));
	}
	else if (ft_strlen(&line[i]) >= 1 && is_single_op(line[i]))
	{
		str_append_n(&s, &line[i], 1);
		*index = i + 1;
		return (tok(TOK_OP, s.data));
	}

	while (i < ft_strlen(line) && !isspace(line[i]) && line[i] != '<'
		&& line[i] != '>' && line[i] != '|' && line[i] != '('
		&& line[i] != ')' && ft_strncmp(&line[i], "&&", 2))
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			*index = i;
			i++;
			while (line[i])
			{
				if ((line[i] == '"' && line[*index] == '"'))
					break ;
				else if ((line[i] == '\'' && line[*index] == '\''))
					break ;
				if (line[i] == '\\' && line[*index] == '"'
						&& (line[i + 1] == '"' || line[i + 1] == '\''))
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
			str_append_n(&s, &line[i + 1], 1);
			i++;
		}
		else
		{
			str_append_n(&s, &line[i], 1);
		}
		i++;
	}

	*index = i;
	return (tok(tt, s.data));
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
