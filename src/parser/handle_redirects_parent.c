/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirects_parent.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:02:39 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/22 23:09:43 by ledelbec         ###   ########.fr       */
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

int	open_redirect(char *filename, char *redirect)
{
	int	fd;

	if (!ft_strcmp(redirect, ">"))
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (!ft_strcmp(redirect, ">>"))
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
	else if (!ft_strcmp(redirect, "<"))
		fd = open(filename, O_RDONLY);
	else
		fd = -1;
	if (fd != -1)
		close(fd);
	else
		msh_errno(filename);
	return (fd);
}

static int	handle_outfile(t_node *node, t_tok *tokens, size_t i)
{
	if (!isvalidfile(tokens[i + 1].s))
		return (-2);
	if (open_redirect(tokens[i + 1].s, tokens[i].s) == -1)
		return (-1);
	if (node->cmd.outfile)
		free(node->cmd.outfile);
	node->cmd.outfile = ft_strdup(tokens[i + 1].s);
	node->cmd.append = !ft_strcmp(tokens[i].s, ">>");
	return (0);
}

static int	handle_infile(t_node *node, t_tok *tokens, size_t i)
{
	if (!isvalidfile(tokens[i + 1].s))
		return (-2);
	if (open_redirect(tokens[i + 1].s, tokens[i].s) == -1)
		return (-1);
	if (node->cmd.infile)
		free(node->cmd.infile);
	node->cmd.infile = ft_strdup(tokens[i + 1].s);
	return (0);
}

int	handle_redirects(t_node *node, t_tok *tokens, size_t i)
{
	const size_t	size = ft_vector_size(tokens);
	char			*s;

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
		s = heredoc(tokens[i + 1].s);
		node->cmd.heredoc = true;
		if (!s)
			return (-2);
		if (node->cmd.infile)
			free(node->cmd.infile);
		node->cmd.infile = s;
	}
	return (0);
}
