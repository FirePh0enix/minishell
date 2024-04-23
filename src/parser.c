/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:20:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/23 14:30:29 by ledelbec         ###   ########.fr       */
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

static void	free_tokens(t_tok *tokens)
{
	size_t	i;

	i = 0;
	while (i < ft_vector_size(tokens))
		free(tokens[i++].s);
	ft_vector_free(tokens);
}

t_node	*parse_line(t_minishell *msh, char *line)
{
	t_tok	*tokens;
	t_tok	*tokens2;
	t_tok	*tokens3;
	t_node	*expr;

	(void) msh;
	tokens = split_into_tokens(line);
	tokens2 = expand_wildcards(tokens);
	free_tokens(tokens);
	tokens3 = cleanup_tokens(tokens2);
	expr = parse_expr(tokens3, range(0, ft_vector_size(tokens3) - 1), NULL);
	free_tokens(tokens3);
	return (expr);
}

// -----------------------------------------------------------------------------
// Debugging

/*static void	_print_spaces(int layer)
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
}*/
