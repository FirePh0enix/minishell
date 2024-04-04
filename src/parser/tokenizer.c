/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:31:32 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/04 13:32:38 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

char	*next_string(char *line, size_t *index)
{
	size_t	i;
	size_t	size;
	char	*s;
	char	*s2;

	i = *index + 1;
	s = ft_calloc(2, 1);
	if (!s)
		return (NULL);
	s[0] = line[*index];
	size = 1;
	while (i < ft_strlen(line))
	{
		s2 = ft_realloc(s, size + 1, size + 2);
		if (!s2)
			return (free(s), NULL);
		s = s2;
		s[size++] = line[i];
		s[size] = '\0';
		if ((line[*index] == '"' && line[i] == '"')
			|| (line[*index] == '\'' && line[i] == '\''))
		{
			i++;
			break ;
		}
		i++;
	}
	*index = i;
	return (s);
}

char	*next_token(char *line, size_t *index)
{
	char	*s;
	char	*s2;
	size_t	i;
	size_t	size;

	i = *index;
	s = NULL;
	if (line[i] == '\0')
		return (NULL);
	while (line[i] == ' ')
		i++;
	size = 0;
	if (line[i] == '|' || line[i] == '&' || line[i] == '>' || line[i] == '<'
		|| line[i] == '(' || line[i] == ')')
	{
		if (i + 1 < ft_strlen(line)
			&& (!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2) || !ft_strncmp(&line[i], ">>", 2) || !ft_strncmp(&line[i], "<<", 2)))
		{
			s = ft_calloc(1, 3);
			if (!s)
				return (NULL);
			ft_memcpy(s, &line[i], 2);
			i += 2;
		}
		else
		{
			s = ft_calloc(1, 2);
			if (!s)
				return (NULL);
			s[0] = line[i];
			i++;
		}
		*index = i;
		return (s);
	}
	else if (line[i] == '"' || line[i] == '\'')
	{
		*index = i;
		return (next_string(line, index));
	}
	while (i < ft_strlen(line))
	{
		if (line[i] == ' ' || line[i] == '|' || line[i] == '"' || line[i] == '>'
			|| line[i] == '<' || line[i] == '(' || line[i] == ')')
			break ;
		s2 = ft_realloc(s, size + 1, size + 2);
		if (!s2)
			return (free(s), NULL);
		s = s2;
		s[size++] = line[i];
		s[size] = '\0';
		i++;
	}
	*index = i;
	return (s);
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

