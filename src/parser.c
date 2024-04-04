/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:20:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/04 13:35:43 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

// -----------------------------------------------------------------------------
// Wildcard and env expansion

static int	ispathend(int c)
{
	return (c == '"' || c == '|' || c == '(' || c == ')' || c == '{'
		|| c == '}' || c == '[' || c == ']' || c == '+' || c == '-' || c == '*'
		|| c == '/' || c == ' ');
}

static char	*ft_strndup(char *s, size_t n)
{
	char	*s2;

	s2 = ft_calloc(1, n + 1);
	if (!s2)
		return (NULL);
	ft_memcpy(s2, s, n);
	return (s2);
}

static char	*expand_dquotes(t_minishell *minishell, char *tok)
{
	char	*tok2;
	char	*tok2_2;
	size_t	start;
	size_t	i;
	size_t	size;
	char	*env;

	tok2 = ft_calloc(1, 1);
	i = 1;
	size = 0;
	while (1)
	{
		start = i;
		while (tok[i] && tok[i] != '"' && tok[i] != '$')
			i++;
		tok2_2 = ft_realloc(tok2, size + 1, size + 1 + (i - start));
		if (!tok2_2)
			return (free(tok2), NULL);
		tok2 = tok2_2;
		ft_memcpy(tok2 + size, tok + start, i - start);
		size += i - start;
		if (tok[i] == '$')
		{
			start = i + 1;
			while (tok[i] && !ispathend(tok[i]))
				i++;
			tok2_2 = ft_realloc(tok2, size + 1, size + 1 + (i - start));
			if (!tok2_2)
				return (free(tok2), NULL);
			tok2 = tok2_2;
			env = getourenv(minishell, ft_strndup(tok + start, i - start));
			if (!env)
				continue ;
			ft_memcpy(tok2 + size, env, ft_strlen(env));
			size += ft_strlen(env);
			free(env);
		}
		else
			break ;
	}
	return (tok2);
}

/*
 * Expand metacharacters like *, ~ and $
 */
static char	**expand_tokens(t_minishell *msh, char **tokens)
{
	size_t	i;
	size_t	j;
	char	**tokens2;
	char	**files;
	char	*s;
	char	*env;

	tokens2 = ft_vector(sizeof(char *), 0);
	if (!tokens2)
		return (NULL);
	i = 0;
	while (i < ft_vector_size(tokens))
	{
		if (ft_strchr(tokens[i], '*'))
		{
			files = wildcard(ft_strchr(tokens[i], '*') + 1);
			if (!files)
				return (ft_vector_deep_free(tokens2), NULL);
			j = 0;
			while (j < ft_vector_size(files))
				if (!ft_vector_add(&tokens2, &files[j++]))
					return (ft_vector_deep_free(tokens2), NULL);
		}
		else if (tokens[i][0] == '"')
		{
			s = expand_dquotes(msh, tokens[i]);
			if (!ft_vector_add(&tokens2, &s))
				return (ft_vector_deep_free(tokens2), NULL);
			free(tokens[i]);
		}
		else if (tokens[i][0] == '$')
		{
			env = getourenv(msh, tokens[i] + 1);
			if (env && !ft_vector_add(&tokens2, &env))
				return (ft_vector_deep_free(tokens2), NULL);
		}
		else if (tokens[i][0] == '~')
		{
			env = getourenv(msh, "HOME");
			if (env)
			{
				s = ft_strjoin(env, tokens[i] + 1);
				if (!s || !ft_vector_add(&tokens2, &s))
					return (free(env), free(s), ft_vector_deep_free(tokens2),
						NULL);
				free(env);
			}
			else
			{
				s = ft_strdup(tokens[i] + 1);
				if (!s || !ft_vector_add(&tokens2, &s))
					return (free(s), ft_vector_deep_free(tokens2), NULL);
			}
		}
		else if (!ft_vector_add(&tokens2, &tokens[i]))
			return (ft_vector_deep_free(tokens2), NULL);
		i++;
	}
	return (tokens2);
}

// -----------------------------------------------------------------------------
// Line parsing

t_node	*parse_line(t_minishell *msh, char *line)
{
	char	**tokens;

	msh->heredocs = 0;
	tokens = split_into_tokens(line);
	/*for (size_t i = 0; i < ft_vector_size(tokens); i++)
		free(tokens[i]);
	ft_vector_free(tokens);*/
	tokens = expand_tokens(msh, tokens);
	for (size_t i = 0; i < ft_vector_size(tokens); i++)
		printf("tok: %s\n", tokens[i]);
	return (parse_expr(msh, tokens, 0, ft_vector_size(tokens) - 1));
}

// -----------------------------------------------------------------------------
// Convert "Prenodes" to actual nodes

// -----------------------------------------------------------------------------
// Debugging

static void	_print_spaces(int layer)
{
	int	i;

	i = 0;
	while (i < layer * 3)
	{
		printf(" ");
		i++;
	}
}

static void	_rec_dump_line(t_node *node, int layer)
{
	if (node->type == TY_CMD)
	{
		printf("CMD {\n");
		_print_spaces(layer + 1);
		printf("argv = [ ");
		for (int i = 0; i < node->cmd.argc; i++)
			printf("%s ", node->cmd.argv[i]);
		printf("]\n");
		_print_spaces(layer + 1);
		printf("in = %s\n", node->cmd.infile);
		_print_spaces(layer + 1);
		printf("out = %s\n", node->cmd.outfile);
		_print_spaces(layer + 1);
		printf("append mode = %d\n", node->cmd.append);
		_print_spaces(layer);
		printf("}\n");
	}
	else if (node->type == TY_PIPE)
	{
		printf("PIPE {\n");
		_print_spaces(layer + 1);
		printf("left = ");
		_rec_dump_line(node->pipe.left, layer + 1);
		_print_spaces(layer + 1);
		printf("right = ");
		_rec_dump_line(node->pipe.right, layer + 1);
		_print_spaces(layer);
		printf("}\n");
	}
	else if (node->type == TY_OR)
	{
		printf("OR {\n");
		_print_spaces(layer + 1);
		printf("left = ");
		_rec_dump_line(node->pipe.left, layer + 1);
		_print_spaces(layer + 1);
		printf("right = ");
		_rec_dump_line(node->pipe.right, layer + 1);
		_print_spaces(layer);
		printf("}\n");
	}
	else if (node->type == TY_AND)
	{
		printf("AND {\n");
		_print_spaces(layer + 1);
		printf("left = ");
		_rec_dump_line(node->pipe.left, layer + 1);
		_print_spaces(layer + 1);
		printf("right = ");
		_rec_dump_line(node->pipe.right, layer + 1);
		_print_spaces(layer);
		printf("}\n");
	}
}

void	dump_line(t_node *node)
{
	_rec_dump_line(node, 0);
}
