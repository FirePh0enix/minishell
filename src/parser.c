/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:20:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/12 13:34:31 by ledelbec         ###   ########.fr       */
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

static char	*ft_strndup(char *s, size_t n)
{
	char	*s2;

	s2 = ft_calloc(1, n + 1);
	if (!s2)
		return (NULL);
	ft_memcpy(s2, s, n);
	return (s2);
}

static void	write_env(t_minishell *msh, t_str *s, size_t *index, t_str tok)
{
	size_t	i;
	size_t	i2;
	char	*env;
	char	*envname;

	i = *index;
	i++;
	if (tok.data[i] == '?')
	{
		*index = i + 1;
		env = getourenv(msh, "?");
		str_append(s, env);
		free(env);
		return ;
	}
	else if (ft_isdigit(tok.data[i]))
	{
		*index = i;
		return ;
	}
	else if (!ft_isalpha(tok.data[i]))
	{
		str_append(s, "$");
		*index = i;
		return ;
	}
	i++;
	i2 = i;
	while (tok.data[i2] &&
		(ft_isalnum(tok.data[i2]) || tok.data[i2] == '-' || tok.data[i2] == '_'))
		i2++;
	envname = ft_strndup(&tok.data[i - 1], i2 - (i - 1));
	if (!envname)
		return ;
	env = getourenv(msh, envname);
	if (env)
		str_append(s, env);
	*index = i2 - 1;
	free(envname);
	free(env);
}

static t_str	expand_reg(t_minishell *msh, t_str tok)
{
	t_str	s;
	size_t	i;

	s = str("");
	i = 0;
	while (i < tok.size)
	{
		if (tok.data[i] == '"')
		{
			i++;
			while (tok.data[i] && tok.data[i] != '"')
			{
				if (tok.data[i] == '$')
					write_env(msh, &s, &i, tok);
				else
					str_append_n(&s, &tok.data[i++], 1);
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
		else if (tok.data[i] == '$')
			write_env(msh, &s, &i, tok);
		else
			str_append_n(&s, &tok.data[i], 1);
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
	// TODO/FIXME:
	// - First expand `~`
	// - Then env variables
	// - Then wildcards
	while (i < ft_vector_size(tokens))
	{
		tok = tokens[i];

		if (tok.data[0] == '~')
		{
			env = getourenv(msh, "HOME");
			if (env)
			{
				tok = str(env);
				str_append(&tok, tokens[i].data + 1);
				free(env);
			}
			else
			{
				// FIXME: What to do here ?
				return (NULL);
			}
		}

		if (ft_strchr(tok.data, '*'))
		{
			files = wildcard(tok.data, 0);
			if (!files)
				return (ft_vector_deep_free(tokens2), NULL);
			if (ft_vector_size(files) == 0)
			{
				s = ft_strdup(tok.data);
				ft_vector_add(&tokens2, &s);
			}
			else
			{
				j = 0;
				while (j < ft_vector_size(files))
					if (!ft_vector_add(&tokens2, &files[j++]))
						return (ft_vector_deep_free(tokens2), NULL);
			}
		}
		else
		{
			tok = expand_reg(msh, tok);
			if (!tok.data)
				return (str_free(&tok), NULL);
			// if (tok.size == 0)
			//	str_free(&tok);
			// FIXME: Should not add the token if empty expect for dquotes.
			else if (!ft_vector_add(&tokens2, &tok))
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
	//for (size_t i = 0; i < ft_vector_size(tokens2); i++)
	//	ft_fprintf(2, "tok: %s\n", tokens2[i]);
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
		ft_fprintf(2, " ");
		i++;
	}
}

static void	_rec_dump_line(t_node *node, int layer)
{
	if (node->type == TY_CMD)
	{
		ft_fprintf(2, "CMD {\n");
		_print_spaces(layer + 1);
		ft_fprintf(2, "argv = [ ");
		for (int i = 0; i < node->cmd.argc; i++)
			ft_fprintf(2, "%s ", node->cmd.argv[i]);
		ft_fprintf(2, "]\n");
		_print_spaces(layer + 1);
		ft_fprintf(2, "in = %s\n", node->cmd.infile);
		_print_spaces(layer + 1);
		ft_fprintf(2, "out = %s\n", node->cmd.outfile);
		_print_spaces(layer + 1);
		ft_fprintf(2, "append mode = %d\n", node->cmd.append);
		_print_spaces(layer);
		ft_fprintf(2, "}\n");
	}
	else if (node->type == TY_PIPE)
	{
		ft_fprintf(2, "PIPE {\n");
		_print_spaces(layer + 1);
		ft_fprintf(2, "left = ");
		_rec_dump_line(node->pipe.left, layer + 1);
		_print_spaces(layer + 1);
		ft_fprintf(2, "right = ");
		_rec_dump_line(node->pipe.right, layer + 1);
		_print_spaces(layer);
		ft_fprintf(2, "}\n");
	}
	else if (node->type == TY_OR)
	{
		ft_fprintf(2, "OR {\n");
		_print_spaces(layer + 1);
		ft_fprintf(2, "left = ");
		_rec_dump_line(node->pipe.left, layer + 1);
		_print_spaces(layer + 1);
		ft_fprintf(2, "right = ");
		_rec_dump_line(node->pipe.right, layer + 1);
		_print_spaces(layer);
		ft_fprintf(2, "}\n");
	}
	else if (node->type == TY_AND)
	{
		ft_fprintf(2, "AND {\n");
		_print_spaces(layer + 1);
		ft_fprintf(2, "left = ");
		_rec_dump_line(node->pipe.left, layer + 1);
		_print_spaces(layer + 1);
		ft_fprintf(2, "right = ");
		_rec_dump_line(node->pipe.right, layer + 1);
		_print_spaces(layer);
		ft_fprintf(2, "}\n");
	}
}

void	dump_line(t_node *node)
{
	_rec_dump_line(node, 0);
}
