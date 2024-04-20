/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:13:33 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/20 19:14:46 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

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
