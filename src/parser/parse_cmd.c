/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:05:20 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/22 16:55:41 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"
#include <stdbool.h>

int		handle_redirects(t_node *node, t_tok *tokens, size_t i);

static void	read_vars(t_node *node, t_tok *tokens, size_t *index, size_t end)
{
	size_t	i;
	char	*s;
	char	*s2;
	char	*s3;

	i = *index;
	while (i <= end)
	{
		s = ft_strchr(tokens[i].s, '=');
		if (!s)
			break ;
		s2 = ft_strndup(tokens[i].s, s - tokens[i].s);
		if (!is_valid_var_name(s2))
		{
			free(s2);
			break ;
		}
		s3 = ft_strdup(tokens[i].s);
		ft_vector_add(&node->cmd.env, &s3);
		free(s2);
		i++;
	}
	*index = i;
}

static bool	is_redirect(char *tok)
{
	return (!ft_strcmp(tok, ">") || !ft_strcmp(tok, ">>")
		|| !ft_strcmp(tok, "<") || !ft_strcmp(tok, "<<"));
}

static void	*_parse_cmd2(t_node *node, t_tok *tokens, t_range r)
{
	size_t	i;
	char	*tok;
	int		err;

	i = r.start;
	read_vars(node, tokens, &i, r.end);
	while (i <= r.end)
	{
		tok = ft_strdup(tokens[i].s);
		if (tokens[i].type == TOK_OP && is_redirect(tok))
		{
			err = handle_redirects(node, tokens, i);
			if (err == -1)
				return (free(tok), (void *)1);
			else if (err == -2)
				return (free(tok), NULL);
			i++;
			free(tok);
		}
		else if (!ft_vector_add(&node->cmd.argv, &tok))
			return (free_node(node), free(tok), NULL);
		i++;
	}
	return (node);
}

t_node	*parse_cmd(t_tok *tokens, t_range r, t_node *parent)
{
	t_node	*node;
	char	*tok;
	void	*p;

	node = ft_calloc(sizeof(t_node), 1);
	if (!node)
		return (NULL);
	node->type = TY_CMD;
	node->parent = parent;
	node->cmd.argv = ft_vector(sizeof(char *), 0);
	node->cmd.env = ft_vector(sizeof(char *), 0);
	if (!node->cmd.argv || !node->cmd.env)
		return (free(node), NULL);
	p = _parse_cmd2(node, tokens, r);
	if (p == NULL || p == (void *)1)
		return (free_node(node), p);
	tok = NULL;
	if (!ft_vector_add(&node->cmd.argv, &tok))
		return (ft_vector_deep_free(node->cmd.argv), free(node), NULL);
	node->cmd.argc = ft_vector_size(node->cmd.argv) - 1;
	if (node->cmd.argc == 0 && !node->cmd.outfile && !node->cmd.infile
		&& ft_vector_size(node->cmd.env) == 0)
		return (free_node(node), NULL);
	return (node);
}
