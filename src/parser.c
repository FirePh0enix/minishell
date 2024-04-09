/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:20:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/09 14:32:41 by ledelbec         ###   ########.fr       */
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

static int	isnameend(int c)
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

static t_str	expand_reg(t_minishell *msh, t_str tok)
{
	t_str	s;
	size_t	i;
	size_t	i2;
	char	*envname;
	char	*env;

	s = str("");
	i = 0;
	while (tok.data[i])
	{
		if (tok.data[i] == '"')
		{
			i++;
			while (tok.data[i] && tok.data[i] != '"')
			{
				if (tok.data[i] == '$')
				{
					i++;
					if (!ft_isalpha(tok.data[i]))
					{
						i++;
						continue ;
					}
					i++;
					i2 = i;
					while (tok.data[i2] &&
						(ft_isalnum(tok.data[i2]) || tok.data[i2] == '-' || tok.data[i2] == '_'))
						i2++;
					envname = ft_strndup(&tok.data[i - 1], i2 - (i - 1));
					env = getourenv(msh, envname);
					str_append(&s, env);
					i = i2;
					free(envname);
					free(env);
				}
				else
				{
					str_append_n(&s, &tok.data[i], 1);
					i++;
				}
			}
			if (tok.data[i] != '"')
				return (str_free(&s), str_null());
		}
		else if (tok.data[i] == '\'')
		{
			i++;
			while (tok.data[i] && tok.data[i] != '\'')
			{
				str_append_n(&s, &tok.data[i], 1);
				i++;
			}
			if (tok.data[i] != '\'')
				return (str_free(&s), str_null());
		}
		else
		{
			str_append_n(&s, &tok.data[i], 1);
		}
		i++;
	}
	return (s);
}

/*
 * Expand metacharacters like *, ~ and $
 */
static char	**expand_tokens(t_minishell *msh, t_str *tokens)
{
	size_t	i;
	size_t	j;
	char	**tokens2;
	char	**files;
	char	*s;
	char	*env;
	t_str	tok;

	tokens2 = ft_vector(sizeof(char *), 0);
	if (!tokens2)
		return (NULL);
	i = 0;
	while (i < ft_vector_size(tokens))
	{
		if (ft_strchr(tokens[i].data, '*'))
		{
			files = wildcard(ft_strchr(tokens[i].data, '*') + 1);
			if (!files)
				return (ft_vector_deep_free(tokens2), NULL);
			j = 0;
			while (j < ft_vector_size(files))
				if (!ft_vector_add(&tokens2, &files[j++]))
					return (ft_vector_deep_free(tokens2), NULL);
		}
		else if (tokens[i].data[0] == '$')
		{
			env = getourenv(msh, tokens[i].data + 1);
			if (env && !ft_vector_add(&tokens2, &env))
				return (ft_vector_deep_free(tokens2), NULL);
		}
		else if (tokens[i].data[0] == '~')
		{
			env = getourenv(msh, "HOME");
			if (env)
			{
				s = ft_strjoin(env, tokens[i].data + 1);
				if (!s || !ft_vector_add(&tokens2, &s))
					return (free(env), free(s), ft_vector_deep_free(tokens2),
						NULL);
				free(env);
			}
			else
			{
				s = ft_strdup(tokens[i].data + 1);
				if (!s || !ft_vector_add(&tokens2, &s))
					return (free(s), ft_vector_deep_free(tokens2), NULL);
			}
		}
		else
		{
			tok = expand_reg(msh, tokens[i]);
			if (!tok.data)
				return (str_free(&tok), NULL);
			if (!ft_vector_add(&tokens2, &tok))
				return (ft_vector_deep_free(tokens2), NULL);
		}
		i++;
	}
	return (tokens2);
}

// -----------------------------------------------------------------------------
// Line parsing

t_node	*parse_line(t_minishell *msh, char *line)
{
	t_str	*tokens;
	char	**tokens2;
	t_node	*expr;

	msh->heredocs = 0;
	tokens = split_into_tokens(line);
	tokens2 = expand_tokens(msh, tokens);
	ft_vector_deep_free(tokens);
	for (size_t i = 0; i < ft_vector_size(tokens2); i++)
		printf("tok: %s\n", tokens2[i]);
	expr = parse_expr(msh, tokens2, 0, ft_vector_size(tokens2) - 1);
	ft_vector_deep_free(tokens2);
	return (expr);
}

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
