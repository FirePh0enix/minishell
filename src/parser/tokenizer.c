/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:31:32 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/28 12:20:59 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int	read_string(t_str *s, char *line, size_t *index)
{
	size_t	i;

	i = *index;
	i++;
	while (line[i])
	{
		if ((line[i] == '"' && line[*index] == '"'))
			break ;
		else if ((line[i] == '\'' && line[*index] == '\''))
			break ;
		if (line[i] == '\\' && line[*index] == '"'
			&& (line[i + 1] == '"' || line[i + 1] == '\''))
			str_append_n(s, &line[i++ + 1], 1);
		else
			str_append_n(s, &line[i], 1);
		i++;
	}
	if (line[i] == '\0')
		return (-1);
	*index = i;
	return (0);
}

int	read_token(t_str *s, char *line, size_t *index)
{
	size_t	i;

	i = *index;
	while (i < ft_strlen(line) && !ft_isspace(line[i]) && !is_single_op(line[i])
		&& ft_strncmp(&line[i], "&&", 2))
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			if (read_string(s, line, &i) == -1)
				return (-1);
		}
		else if (line[i] == '\\' && line[i + 1] == ' ')
			str_append_n(s, &line[i++ + 1], 1);
		else
			str_append_n(s, &line[i], 1);
		i++;
	}
	*index = i;
	return (0);
}

static t_tok	next_token(char *line, size_t *index, int *err)
{
	t_str		s;
	size_t		i;
	t_tok_type	tt;

	i = *index;
	if (i >= ft_strlen(line))
		return (nulltok());
	while (line[i] && ft_isspace(line[i]))
		i++;
	*index = i;
	if (line[i] == '\0')
		return (nulltok());
	s = str("");
	tt = TOK_IDENT;
	if (ft_strlen(&line[i]) >= 2 && is_dual_op(&line[i]))
		return (str_append_n(&s, &line[i], 2), *index = i + 2,
			tok(TOK_OP, s.data));
	else if (ft_strlen(&line[i]) >= 1 && is_single_op(line[i]))
		return (str_append_n(&s, &line[i], 1), *index = i + 1,
			tok(TOK_OP, s.data));
	if (read_token(&s, line, &i) == -1)
	{
		str_free(&s);
		*err = 1;
		return (nulltok());
	}
	*index = i;
	return (tok(tt, s.data));
}

t_tok	*split_into_tokens(char *line)
{
	size_t	index;
	t_tok	*tokens;
	t_tok	tok;
	int		err;

	err = 0;
	tokens = ft_vector(sizeof(t_tok), 0);
	if (!tokens)
		return (NULL);
	index = 0;
	while (1)
	{
		tok = next_token(line, &index, &err);
		if (!tok.s)
			break ;
		if (!ft_vector_add(&tokens, &tok))
			return (free_tokens(tokens), NULL);
	}
	if (err != 0)
		return (free_tokens(tokens), NULL);
	return (tokens);
}
