/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:20:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/03 13:15:35 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

static char	*next_string(char *line, size_t *index)
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

static char	*next_token(char *line, size_t *index)
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
		if(!ft_vector_add(&tokens, &tok))
			return (ft_vector_deep_free(tokens), NULL);
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
// Expression parsing

static int	isop(char *s)
{
	return (!strcmp(s, "|") || !strcmp(s, "||") || !strcmp(s, "&&"));
}

static int	get_op_priority(char *s)
{
	if (!strcmp(s, "|"))
		return (1);
	else if (!strcmp(s, "||") || !strcmp(s, "&&"))
		return (2);
	return (-1);
}

static int	get_op(char **tokens, size_t start, size_t end)
{
	int		i;
	char	*tok;
	int		hprio;
	int		pos;
	int		parent;

	pos = -1;
	i = end;
	hprio = 0;
	parent = 0;
	while (i >= (int) start)
	{
		tok = tokens[i];
		if (!strcmp(tok, ")"))
			parent++;
		else if (!strcmp(tok, "("))
			parent--;
		else if (parent == 0 && isop(tok) && get_op_priority(tok) > hprio)
		{
			hprio = get_op_priority(tok);
			pos = i;
		}
		i--;
	}
	return (pos);
}

static char	*heredoc(t_minishell *msh, char *eof)
{
	char		*line;
	int			fd;
	char		filename[256];

	ft_sprintf(filename, "/tmp/msh-miniseashell-heredoc-%zu", msh->heredocs++);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!strcmp(line, eof))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	close(fd);
	return (ft_strdup(filename));
}

static t_node	*parse_cmd(t_minishell *msh, char **tokens,
		size_t start, size_t end)
{
	t_node	*node;
	size_t	i;
	char	*tok;

	node = ft_calloc(sizeof(t_node), 1);
	if (!node)
		return (NULL);
	node->type = TY_CMD;
	node->cmd.argv = ft_vector(sizeof(char *), 0);
	if (!node->cmd.argv)
		return (free(node), NULL);
	i = start;
	while (i <= end)
	{
		tok = tokens[i];
		if (!strcmp(tok, "<"))
		{
			if (i + 1 > end)
				return (NULL);
			i++;
			node->cmd.infile = tokens[i];
		}
		else if (!strcmp(tok, "<<"))
		{
			if (i + 1 > end - start)
				return (NULL);
			i++;
			if (node->cmd.infile)
				free(node->cmd.infile);
			node->cmd.infile = heredoc(msh, tokens[i]);
			if (!node->cmd.infile)
				return (ft_vector_deep_free(node->cmd.argv), free(node), NULL);
		}
		else if (!strcmp(tok, ">") || !strcmp(tok, ">>"))
		{
			if (i + 1 > end)
				return (NULL);
			i++;
			node->cmd.outfile = tokens[i];
			node->cmd.append = !strcmp(tok, ">>");
		}
		else if (!ft_vector_add(&node->cmd.argv, &tok))
			return (ft_vector_deep_free(node->cmd.argv), free(node), NULL);
		i++;
	}
	tok = NULL;
	if (!ft_vector_add(&node->cmd.argv, &tok))
		return (ft_vector_deep_free(node->cmd.argv), free(node), NULL);
	node->cmd.argc = ft_vector_size(node->cmd.argv) - 1;
	return (node);
}

static t_node	*parse_expr(t_minishell *msh, char **tokens, size_t start, size_t end);

static void	apply_out(t_minishell *msh, t_node *node, char *outfile, bool append)
{
	if (node->type == TY_CMD)
	{
		node->cmd.outfile = outfile;
		node->cmd.append = append;
	}
	else if (node->type == TY_PIPE)
	{
		apply_out(msh, node->pipe.right, outfile, append);
	}
	else if (node->type == TY_AND || node->type == TY_OR)
	{
		// Append mode is forced on the right node to simulate the behaviour
		// of bash
		apply_out(msh, node->pipe.left, outfile, append);
		apply_out(msh, node->pipe.right, outfile, true);
	}
}

static void	apply_in(t_minishell *msh, t_node *node, char *infile)
{
	if (node->type == TY_CMD)
	{
		node->cmd.infile = infile;
	}
	else if (node->type == TY_PIPE)
	{
		apply_in(msh, node->pipe.left, infile);
	}
	else if (node->type == TY_AND || node->type == TY_OR)
	{
		apply_in(msh, node->pipe.left, infile);
		apply_in(msh, node->pipe.right, infile);
	}
}

static t_node	*parse_parent(t_minishell *msh, char **tokens, size_t start, size_t end)
{
	char	*outfile = NULL;
	char	*infile = NULL;
	bool	append;

	// First find the parenthesis start and end values
	int	parent_start;
	int	parent_end;

	parent_start = - 1;
	parent_end = -1;
	for (size_t i = start; i <= end; i++)
	{
		if (!strcmp(tokens[i], "("))
		{
			if (parent_start != -1)
				return (NULL);
			parent_start = i;
		}
		else if (!strcmp(tokens[i], ")"))
		{
			if (parent_end != -1)
				return (NULL);
			parent_end = i;
		}
	}

	// printf("parent: %d %d\n", parent_start, parent_end);
	// printf("        %zd %zd\n", start, end);

	if (parent_start == -1 && parent_end == -1)
		return (parse_cmd(msh, tokens, start, end));
	else if (parent_start == -1 || parent_end == -1)
		return (NULL);

	// Then parse redirections here
	char	*tok;

	for (size_t i = start; i < end; i++)
	{
		if (i >= (size_t)parent_start && i <= (size_t)parent_end)
			continue ;
		tok = tokens[i];
		if (!strcmp(tok, "<"))
		{
			if (i + 1 > end - start)
				return (NULL);
			i++;
			infile = tokens[i + start];
		}
		else if (!strcmp(tok, "<<"))
		{
			if (i + 1 > end - start)
				return (NULL);
			i++;
			infile = heredoc(msh, tokens[i]);
			if (!infile)
				return (NULL); // free outfile, infile if needed
		}
		else if (!strcmp(tok, ">") || !strcmp(tok, ">>"))
		{
			if (i + 1 > end)
				return (NULL);
			i++;
			outfile = tokens[i + start];
			append = !strcmp(tok, ">>");
		}
	}

	// Finally parse what's between parenthesis and add the redirections

	t_node	*node = parse_expr(msh, tokens, parent_start + 1, parent_end - 1);

	if (outfile != NULL)
		apply_out(msh, node, outfile, append);
	if (infile != NULL)
		apply_in(msh, node, infile);
	return (node);
}

static t_node	*parse_expr(t_minishell *msh, char **tokens, size_t start, size_t end)
{
	int			pos;
	t_node	*node;

	pos = get_op(tokens, start, end);
	if (pos == -1)
		return (parse_parent(msh, tokens, start, end));
	else if (pos == 0)
		return (NULL);
	else if (!strcmp(tokens[pos], "|"))
	{
		node = malloc(sizeof(t_node));
		if (!node)
			return (NULL);
		node->type = TY_PIPE;
		node->pipe.left = parse_expr(msh, tokens, start, pos - 1);
		node->pipe.right = parse_expr(msh, tokens, pos + 1, end);
		return (node);
	}
	else if (!strcmp(tokens[pos], "||"))
	{
		node = malloc(sizeof(t_node));
		if (!node)
			return (NULL);
		node->type = TY_OR;
		node->pipe.left = parse_expr(msh, tokens, start, pos - 1);
		node->pipe.right = parse_expr(msh, tokens, pos + 1, end);
		return (node);
	}
	else if (!strcmp(tokens[pos], "&&"))
	{
		node = malloc(sizeof(t_node));
		if (!node)
			return (NULL);
		node->type = TY_AND;
		node->pipe.left = parse_expr(msh, tokens, start, pos - 1);
		node->pipe.right = parse_expr(msh, tokens, pos + 1, end);
		return (node);
	}
	return (NULL);
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
