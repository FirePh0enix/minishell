/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:31:32 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/09 13:49:58 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

t_str	next_token(char *line, size_t *index)
{
	t_str	s;
	size_t	i;

	i = *index;
	s = str("");
	if (line[i] == '\0')
		return (str_null());
	while (line[i] == ' ')
		i++;
	if (line[i] == '|' || line[i] == '&' || line[i] == '>' || line[i] == '<'
		|| line[i] == '(' || line[i] == ')')
	{
		if (i + 1 < ft_strlen(line)
			&& (!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2) || !ft_strncmp(&line[i], ">>", 2) || !ft_strncmp(&line[i], "<<", 2)))
		{
			if (!str_append_n(&s, &line[i], 2))
				return (str_free(&s), str_null());
			i += 2;
		}
		else
		{
			if (!str_append_n(&s, &line[i], 1))
				return (str_free(&s), str_null());
			i++;
		}
		*index = i;
		return (s);
	}
	while (i < ft_strlen(line))
	{
		if (line[i] == ' ' || line[i] == '|' || line[i] == '>'
			|| line[i] == '<' || line[i] == '(' || line[i] == ')')
			break ;
		if (!str_append_n(&s, &line[i], 1))
			return (str_free(&s), str_null());
		if (line[i] == '"')
		{
			i++;
			while (i < ft_strlen(line) && line[i] != '"')
			{
				if (!str_append_n(&s, &line[i], 1))
					return (str_free(&s), str_null());
				i++;
			}
			if (!str_append(&s, "\""))
				return (str_free(&s), str_null());
		}
		else if (line[i] == '\'')
		{
			i++;
			while (i < ft_strlen(line) && line[i] != '\'')
			{
				if (!str_append_n(&s, &line[i], 1))
					return (str_free(&s), str_null());
				i++;
			}
			if (!str_append(&s, "'"))
				return (str_free(&s), str_null());
		}
		i++;
	}
	*index = i;
	return (s);
}

t_str	*split_into_tokens(char *line)
{
	size_t	index;
	t_str	*tokens;
	t_str	tok;

	tokens = ft_vector(sizeof(t_str), 0);
	if (!tokens)
		return (NULL);
	index = 0;
	while (1)
	{
		tok = next_token(line, &index);
		if (!tok.data)
			break ;
		if(!ft_vector_add(&tokens, &tok.data))
			return (ft_vector_deep_free(tokens), NULL);
	}
	return (tokens);
}

