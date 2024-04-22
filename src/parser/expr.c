/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:34:59 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/22 23:11:14 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include <string.h>
#include <readline/readline.h>

int		get_op(t_tok *tokens, t_range range);
int		handle_redirects(t_node *node, t_tok *tokens, size_t i);
t_node	*parse_cmd(t_tok *tokens, t_range r, t_node *parent);

void	apply_out(t_node *node, char *outfile, bool append);
void	apply_in(t_node *node, char *infile);

bool	is_redirect(char *tok, t_tok_type type);
bool	is_invalid(int parent_start, int parent_end, t_range r);

static t_node	*handle_all_redirects(t_tok *tokens, t_range r,
	int parent_start, int parent_end)
{
	size_t	i;
	t_node	*n;
	char	*tok;

	n = ft_calloc(1, sizeof(t_node));
	i = r.start;
	while (i <= r.end)
	{
		if ((int)i >= parent_start && (int)i <= parent_end)
		{
			i++;
			continue ;
		}
		tok = tokens[i].s;
		if (is_redirect(tok, tokens[i].type))
		{
			if (handle_redirects(n, tokens, i++) == -1)
				return (free(n), (void *) 1);
		}
		else
			return (free(n), NULL);
		i++;
	}
	return (n);
}

static t_node	*make_parent_node(t_tok *tokens, t_range r, t_range pr,
	t_node *parent)
{
	t_node	*n;
	t_node	*node;
	t_node	*pa;

	n = handle_all_redirects(tokens, r, pr.start, pr.end);
	if (!n)
		return (NULL);
	node = parse_expr(tokens, range(pr.start + 1, pr.end - 1), NULL);
	if (!node)
		return (free(n), NULL);
	if (n->cmd.outfile != NULL)
		apply_out(node, n->cmd.outfile, n->cmd.append);
	if (n->cmd.infile != NULL)
		apply_in(node, n->cmd.infile);
	free(n);
	pa = ft_calloc(1, sizeof(t_node));
	if (!pa)
		return (NULL);
	pa->type = TY_PARENT;
	pa->parent = parent;
	pa->pa.node = node;
	node->parent = pa;
	return (pa);
}

static int	search_parents(int *parent_start, int *parent_end,
	t_range r, t_tok *tokens)
{
	size_t	i;
	int		open_parents;

	open_parents = 0;
	i = r.start;
	while (i <= r.end)
	{
		if (tokens[i].type == TOK_OP && !ft_strcmp(tokens[i].s, "("))
		{
			if (*parent_start == -1)
				*parent_start = i;
			else
				open_parents += 1;
		}
		else if (tokens[i].type == TOK_OP && !ft_strcmp(tokens[i].s, ")"))
		{
			if (open_parents == 0 && *parent_end == -1)
				*parent_end = i;
			else
				open_parents -= 1;
		}
		i++;
	}
	return (open_parents);
}

static t_node	*parse_parent(t_tok *tokens, t_range r, t_node *parent)
{
	int		parent_start;
	int		parent_end;
	int		open_parents;
	t_node	*pa;

	parent_start = -1;
	parent_end = -1;
	open_parents = search_parents(&parent_start, &parent_end, r, tokens);
	if (open_parents != 0)
		return (NULL);
	if (parent_start == -1 && parent_end == -1)
		return (parse_cmd(tokens, r, parent));
	else if (is_invalid(parent_start, parent_end, r))
		return (NULL);
	pa = make_parent_node(tokens, r, range(parent_start, parent_end), parent);
	return (pa);
}

t_type	type_for_str(char *s);

#define RG range

t_node	*parse_expr(t_tok *tokens, t_range r, t_node *parent)
{
	int		pos;
	t_node	*node;

	pos = get_op(tokens, r);
	if (pos == -1)
		return (parse_parent(tokens, r, parent));
	else if (pos == 0)
		return (NULL);
	else if (!ft_strcmp(tokens[pos].s, "|") || !ft_strcmp(tokens[pos].s, "||")
		|| !ft_strcmp(tokens[pos].s, "&&"))
	{
		node = ft_calloc(1, sizeof(t_node));
		if (!node)
			return (NULL);
		node->type = type_for_str(tokens[pos].s);
		node->parent = parent;
		node->pipe.left = parse_expr(tokens, RG(r.start, pos - 1), node);
		if (!node->pipe.left)
			return (free_node(node), NULL);
		node->pipe.right = parse_expr(tokens, range(pos + 1, r.end), node);
		if (!node->pipe.right)
			return (free_node(node), NULL);
		return (node);
	}
	return (NULL);
}
