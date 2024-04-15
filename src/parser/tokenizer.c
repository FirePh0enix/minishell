/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:31:32 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/15 02:06:15 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char	*next_token(char *line, size_t *index)
{
	t_str	s;
	size_t	i;

	i = *index;
	if (i >= ft_strlen(line))
		return (NULL);

	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;

	*index = i;
	if (line[i] == '\0')
		return (NULL);

	s = str("");

	if (ft_strlen(&line[i]) >= 2
		&& (!ft_strncmp(&line[i], "<<", 2) || !ft_strncmp(&line[i], ">>", 2)
			|| !ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2)))
	{
		str_append_n(&s, &line[i], 2);
		*index = i + 2;
		return (s.data);
	}
	else if (ft_strlen(&line[i]) >= 1
		&& (line[i] == '<' || line[i] == '>' || line[i] == '|'
			|| line[i] == '(' || line[i] == ')'))
	{
		str_append_n(&s, &line[i], 1);
		*index = i + 1;
		return (s.data);
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
				{
					// i++;
					break ;
				}
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
	return (s.data);
}

char	**split_into_tokens(char *line)
{
	size_t	index;
	char	**tokens;
	char	*tok;

	tokens = ft_vector(sizeof(char *), 0);
	if (!tokens)
		return (NULL);
	index = 0;
	while (1)
	{
		tok = next_token(line, &index);
		if (!tok)
			break ;
		if(!ft_vector_add(&tokens, &tok))
			return (ft_vector_deep_free(tokens), NULL);
	}
	return (tokens);
}
