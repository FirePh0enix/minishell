/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirects_parent.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:02:39 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/29 11:10:33 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"

char	*heredoc(char *eof);

static bool	isvalidfile(char *s)
{
	return (!(!ft_strcmp(s, "<") || !ft_strcmp(s, ">") || !ft_strcmp(s, "<<")
			|| !ft_strcmp(s, ">>") || !ft_strcmp(s, "|") || !ft_strcmp(s, "||")
			|| !ft_strcmp(s, "&") || !ft_strcmp(s, "&&")));
}

static int	handle_outfile(t_node *node, t_tok *tokens, size_t i)
{
	t_red	red;

	if (!isvalidfile(tokens[i + 1].s))
		return (-2);
	red.filename = ft_strdup(tokens[i + 1].s);
	red.type = RED_OUT;
	red.append = !ft_strcmp(tokens[i].s, ">>");
	ft_vector_add(&node->cmd.all_reds, &red);
	node->cmd.outfile = red.filename;
	node->cmd.append = red.append;
	return (0);
}

static int	handle_infile(t_node *node, t_tok *tokens, size_t i)
{
	t_red	red;

	if (!isvalidfile(tokens[i + 1].s))
		return (-2);
	red.filename = ft_strdup(tokens[i + 1].s);
	red.type = RED_IN;
	ft_vector_add(&node->cmd.all_reds, &red);
	node->cmd.infile = red.filename;
	return (0);
}

int	handle_redirects(t_node *node, t_tok *tokens, size_t i)
{
	const size_t	size = ft_vector_size(tokens);
	t_red			red;

	if (i + 1 >= size)
		return (-1);
	if (!ft_strcmp(tokens[i].s, ">") || !ft_strcmp(tokens[i].s, ">>"))
		return (handle_outfile(node, tokens, i));
	else if (!ft_strcmp(tokens[i].s, "<"))
		return (handle_infile(node, tokens, i));
	else if (!ft_strcmp(tokens[i].s, "<<"))
	{
		if (!isvalidfile(tokens[i + 1].s))
			return (-2);
		red.filename = heredoc(tokens[i + 1].s);
		red.type = RED_IN;
		if (!red.filename)
			return (-2);
		ft_vector_add(&node->cmd.all_reds, &red);
		node->cmd.heredoc = true;
		node->cmd.infile = red.filename;
	}
	return (0);
}
