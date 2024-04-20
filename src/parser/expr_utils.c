/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:10:35 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/20 19:12:21 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	apply_out(t_node *node, char *outfile, bool append)
{
	if (node->type == TY_CMD)
	{
		node->cmd.outfile = ft_strdup(outfile);
		node->cmd.append = append;
	}
	else if (node->type == TY_PIPE)
	{
		apply_out(node->pipe.right, outfile, append);
	}
	else if (node->type == TY_AND || node->type == TY_OR)
	{
		apply_out(node->pipe.left, outfile, append);
		apply_out(node->pipe.right, outfile, true);
	}
}

void	apply_in(t_node *node, char *infile)
{
	if (node->type == TY_CMD)
	{
		node->cmd.infile = ft_strdup(infile);
	}
	else if (node->type == TY_PIPE)
	{
		apply_in(node->pipe.left, infile);
	}
	else if (node->type == TY_AND || node->type == TY_OR)
	{
		apply_in(node->pipe.left, infile);
		apply_in(node->pipe.right, infile);
	}
}

static int	prio(char *s)
{
	if (!ft_strcmp(s, "|"))
		return (1);
	else if (!ft_strcmp(s, "||") || !ft_strcmp(s, "&&"))
		return (2);
	return (-1);
}

int	get_op(t_tok *tokens, t_range range)
{
	int		i;
	char	*tok;
	int		hprio;
	int		pos;
	int		parent;

	pos = -1;
	i = range.start;
	hprio = 0;
	parent = 0;
	while (i <= (int) range.end)
	{
		tok = tokens[i].s;
		if (tokens[i].type == TOK_OP && !ft_strcmp(tok, ")"))
			parent--;
		else if (tokens[i].type == TOK_OP && !ft_strcmp(tok, "("))
			parent++;
		else if (parent == 0 && tokens[i].type == TOK_OP && prio(tok) > hprio)
		{
			hprio = prio(tok);
			pos = i;
		}
		i++;
	}
	return (pos);
}
