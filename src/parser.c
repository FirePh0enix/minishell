/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:20:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/26 16:15:51 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static char	*next_string(char *line, size_t *index)
{
	size_t	i;
	size_t	size;
	char	*s;

	i = *index;
	s = ft_calloc(1, 1);
	size = 0;
	while (i < ft_strlen(line))
	{
		s = ft_realloc(s, size + 1, size + 2);
		s[size++] = line[i];
		s[size] = '\0';
		i++;
		if ((line[*index] == '"' && line[i] == '"') || (line[*index] == '\'' && line[i] == '\''))
		{
			i++;
			break ;
		}
	}
	*index = i;
	return (s);
}

static char	*next_token(char *line, size_t *index)
{
	char	*s;
	size_t	i;
	size_t	size;

	i = *index;
	s = NULL;
	if (line[i] == '\0')
		return (NULL);
	while (line[i] == ' ')
		i++;
	size = 0;
	if (line[i] == '|' || line[i] == '&' || line[i] == '>' || line[i] == '<')
	{
		if (i + 1 < ft_strlen(line)
			&& (!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2) || !ft_strncmp(&line[i], ">>", 2) || !ft_strncmp(&line[i], "<<", 2)))
		{
			s = ft_calloc(1, 3);
			ft_memcpy(s, &line[i], 2);
			i += 2;
		}
		else
		{
			s = ft_calloc(1, 2);
			s[0] = line[i];
			i++;
		}
		*index = i;
		return (s);
	}
	else if (line[i] == '"' || line[i] == '"')
		return (next_string(line, index));
	while (i < ft_strlen(line))
	{
		if (line[i] == ' ' || line[i] == '|' || line[i] == '"' || line[i] == '>' || line[i] == '<')
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

	tokens = ft_vector(sizeof(char *), 0);
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
	size_t	start;
	size_t	i;
	size_t	size;

	tok2 = ft_calloc(1, 1);
	i = 1;
	size = 0;
	while (1)
	{
		start = i;
		while (tok[i] && tok[i] != '$')
			i++;
		tok2 = ft_realloc(tok2, size + 1, size + 1 + (i - start));
		ft_memcpy(tok2 + size, tok + start, i - start);
		size += i - start;
		if (tok[i] == '$')
		{
			start = i + 1;
			while (tok[i] && !ispathend(tok[i]))
				i++;
			tok2 = ft_realloc(tok2, size + 1, size + 1 + (i - start));
			char	*env = getourenv(minishell, ft_strndup(tok + start, i - start));
			if (!env)
				continue ;
			ft_memcpy(tok2 + size, env, ft_strlen(env));
			size += ft_strlen(env);
		}
		else
			break ;
	}
	return (tok2);
}

/*
 * Expand metacharacters like * and $
 */
static char	**expand_tokens(t_minishell *minishell, char **tokens)
{
	size_t	i;
	size_t	j;
	char	**tokens2;
	char	**files;
	char	*s;

	tokens2 = ft_vector(sizeof(char *), 0);
	i = 0;
	while (i < ft_vector_size(tokens))
	{
		if (ft_strchr(tokens[i], '*'))
		{
			files = wildcard(ft_strchr(tokens[i], '*') + 1);
			j = 0;
			while (j < ft_vector_size(files))
				ft_vector_add(&tokens2, &files[j++]);
		}
		else if (tokens[i][0] == '"')
		{
			s = expand_dquotes(minishell, tokens[i]);
			ft_vector_add(&tokens2, &s);
			free(tokens[i]);
		}
		else if (tokens[i][0] == '$')
		{
			char	*env = getourenv(minishell, tokens[i] + 1);
			if (env)
				ft_vector_add(&tokens2, &env);
		}
		else
		{
			ft_vector_add(&tokens2, &tokens[i]);
		}
		i++;
	}
	return (tokens2);
}

// -----------------------------------------------------------------------------
// Expression parsing

static int	isop(char *s)
{
	return (!strcmp(s, "|") || !strcmp(s, ">"));
}

static int	get_op_priority(char *s)
{
	if (!strcmp(s, "|"))
		return (1);
	else if (!strcmp(s, ">"))
		return (2);
	return (-1);
}

static int	get_op(char **tokens, size_t start, size_t end)
{
	int		i;
	char	*tok;
	int		hprio;
	int		pos;

	pos = -1;
	i = end - 1;
	hprio = 0;
	while (i >= (int) start)
	{
		tok = tokens[i];
		if (isop(tok) && get_op_priority(tok) > hprio)
		{
			hprio = get_op_priority(tok);
			pos = i;
		}
		i--;
	}
	return (pos);
}

static t_node	*parse_expr(char **tokens, size_t start, size_t end)
{
	int		pos;
	t_node	*node;
	char	*s;

	node = malloc(sizeof(t_node));
	if (!node)
		return (node);
	pos = get_op(tokens, start, end);
	if (pos == -1)
	{
		node->type = TY_CMD;
		node->cmd.argv = ft_vector(sizeof(char *), 0);
		for (size_t j = 0; j < end - start; j++)
			ft_vector_add(&node->cmd.argv, &tokens[j + start]);
		node->cmd.argc = ft_vector_size(node->cmd.argv);
		s = NULL;
		ft_vector_add(&node->cmd.argv, &s);
		return (node);
	}
	else if (!strcmp(tokens[pos], "|"))
	{
		node->type = TY_PIPE;
		node->pipe.left = parse_expr(tokens, start, pos);
		node->pipe.right = parse_expr(tokens, pos + 1, end);
		return (node);
	}

	// TODO
	//
	// The command `ls > test.txt Makefile | grep Make` should expand to the
	// following tree:
	//
	//               pipe
	//               / \
	//              /   \
	//             /     \
	//            /       \
	//          red       cmd
	//          / \  [ grep Make ]
	//         /   \
	//        /     \
	//      cmd    test.txt
	// [ ls Makefile ]
	//
	// Where the output of `ls Makefile` is redirected to `test.txt` but also
	// piped to `grep Makefile`

	return (NULL);
}

// -----------------------------------------------------------------------------
// Line parsing

t_node	*parse_line(t_minishell *minishell, char *line)
{
	char	**tokens;

	tokens = split_into_tokens(line);
	tokens = expand_tokens(minishell, tokens);
	for (size_t i = 0; i < ft_vector_size(tokens); i++)
		printf("tok: %s\n", tokens[i]);
	return (parse_expr(tokens, 0, ft_vector_size(tokens)));
}

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
}

void	dump_line(t_node *node)
{
	_rec_dump_line(node, 0);
}
