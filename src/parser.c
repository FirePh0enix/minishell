/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:20:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/22 20:07:00 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static char	*next_token(char *line, size_t *index)
{
	char	*s;
	size_t	i;
	size_t	size;

	i = *index;
	s = NULL;
	while (line[i] == ' ')
		i++;
	if (line[i] == '|')
	{
		s = ft_calloc(1, 2);
		s[0] = line[i];
		i++;
		*index = i;
		return (s);
	}
	size = 0;
	while (i < ft_strlen(line))
	{
		if (line[i] == ' ' || line[i] == '|')
			break ;
		s = ft_realloc(s, size + 1, size + 2);
		s[size++] = line[i];
		s[size] = '\0';
		i++;
	}
	*index = i;
	return (s);
}

static char	**split_into_tokens(char *line)
{
	size_t	index;
	char	**tokens;
	char	*tok;

	tokens = ft_vector(sizeof(t_tok), 0);
	if (!tokens)
		return (NULL);
	index = 0;
	while (1)
	{
		tok = next_token(line, &index);
		if (!tok)
			break ;
		// TODO Should check if ft_vector_add fails to reallocate memory
		ft_vector_add(&tokens, &tok);
	}
	return (tokens);
}

static t_node	*parse_expr(char **tokens, size_t start, size_t end)
{
	int		i;
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = TY_PIPE;
	i = end - 1;
	while (i >= 0)
	{
		if (strcmp(tokens[i], "|") == 0)
		{
			node->pipe.left = parse_expr(tokens, start, i);
			node->pipe.right = parse_expr(tokens, start, i);
			break ;
		}
		i--;
	}
	if (i < 0)
	{
		node->type = TY_CMD;
		node->cmd.argv = malloc(sizeof(char *));
		for (size_t j = 0; j < end - start; j++)
			node->cmd.argv[j] = tokens[j + start];
		node->cmd.argc = end - start;
	}
	return (node);
}

t_node	*parse_line(char *line)
{
	char	**tokens;

	tokens = split_into_tokens(line);
	return (parse_expr(tokens, 0, ft_vector_size(tokens)));
}
