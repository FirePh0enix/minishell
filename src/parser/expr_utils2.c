/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:13:33 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/25 11:43:08 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	apply_out(t_node *node, char *outfile, bool append);
void	apply_in(t_node *node, char *infile);

bool	is_redirect(char *tok, t_tok_type type)
{
	return (type == TOK_OP && (!ft_strcmp(tok, ">") || !ft_strcmp(tok, ">>")
			|| !ft_strcmp(tok, "<") || !ft_strcmp(tok, "<<")));
}

bool	is_invalid(int parent_start, int parent_end, t_range r)
{
	return ((parent_start == -1 || parent_end == -1)
		|| (parent_start != (int) r.start));
}

t_type	type_for_str(char *s)
{
	if (!ft_strcmp(s, "|"))
		return (TY_PIPE);
	else if (!ft_strcmp(s, "||"))
		return (TY_OR);
	else if (!ft_strcmp(s, "&&"))
		return (TY_AND);
	return (0);
}

void	apply_redirects(t_node *n, t_node *node)
{
	if (n->cmd.outfile != NULL)
	{
		apply_out(node, n->cmd.outfile, n->cmd.append);
		free(n->cmd.outfile);
	}
	if (n->cmd.infile != NULL)
	{
		apply_in(node, n->cmd.infile);
		free(n->cmd.infile);
	}
}
