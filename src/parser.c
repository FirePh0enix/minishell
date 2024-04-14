/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:20:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/15 01:16:08 by ledelbec         ###   ########.fr       */
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
// Line parsing

t_node	*parse_line(t_minishell *msh, char *line)
{
	char	**tokens;
	t_node	*expr;

	msh->heredocs = 0;
	tokens = split_into_tokens(line);
	tokens = expand_wildcards(tokens);
	//for (size_t i = 0; i < ft_vector_size(tokens); i++)
	//	ft_fprintf(2, "tok: %s\n", tokens[i]);
	expr = parse_expr(msh, tokens, 0, ft_vector_size(tokens) - 1);
	ft_vector_deep_free(tokens);
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
