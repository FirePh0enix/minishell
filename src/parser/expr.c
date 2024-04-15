/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:34:59 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/15 16:06:06 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include <string.h>
#include <readline/readline.h>

static int	isop(char *s)
{
	return (!strcmp(s, "|") || !strcmp(s, "||") || !strcmp(s, "&&"));
}

static int	get_op_priority(char *s)
{
	if (!strcmp(s, "|"))
		return (1);
	else if (!strcmp(s, "||") || !strcmp(s, "&&"))
		return (2);
	return (-1);
}

static int	get_op(char **tokens, size_t start, size_t end)
{
	int		i;
	char	*tok;
	int		hprio;
	int		pos;
	int		parent;

	pos = -1;
	i = end;
	hprio = 0;
	parent = 0;
	while (i >= (int) start)
	{
		tok = tokens[i];
		if (!strcmp(tok, ")"))
			parent++;
		else if (!strcmp(tok, "("))
			parent--;
		else if (parent == 0 && isop(tok) && get_op_priority(tok) > hprio)
		{
			hprio = get_op_priority(tok);
			pos = i;
		}
		i--;
	}
	return (pos);
}

static char	*heredoc(t_minishell *msh, char *eof)
{
	char		*line;
	int			fd;
	char		filename[256];

	ft_sprintf(filename, "/tmp/msh-miniseashell-heredoc-%zu", msh->heredocs++);
	// TODO: Maybe increment files based on which one are available, to allow
	//       multiple minishell running at the same tome.
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		return (NULL);
	// printf("EOF is `%s`\n", eof);
	while (1)
	{
		line = readline("> ");
		if (!strcmp(line, eof))
			break ;
		else if (!line)
		{
			ft_fprintf(2, "msh: warning: here-document delimited by eof"
				"(wanted`%s')", eof);
			break ;
		}
		ft_putendl_fd(line, fd);
	}
	return (close(fd), ft_strdup(filename));
}

// TODO: Adapt for HEREDOC delimiter
//       Check if its works in all cases
static bool	isvalidfile(char *s)
{
	return (!(!strcmp(s, "<") || !strcmp(s, ">") || !strcmp(s, "<<")
		|| !strcmp(s, ">>") || !strcmp(s, "|") || !strcmp(s, "||")
		|| !strcmp(s, "&") || !strcmp(s, "&&")));
}

static t_node	*parse_cmd(t_minishell *msh, char **tokens,
		size_t start, size_t end)
{
	t_node	*node;
	size_t	i;
	char	*tok;
	bool	first_out = true;

	node = ft_calloc(sizeof(t_node), 1);
	if (!node)
		return (NULL);
	node->type = TY_CMD;
	node->cmd.argv = ft_vector(sizeof(char *), 0);
	if (!node->cmd.argv)
		return (free(node), NULL);
	i = start;
	while (i <= end)
	{
		tok = ft_strdup(tokens[i]);
		if (!strcmp(tok, "<"))
		{
			if (i + 1 > end)
				return (NULL);
			i++;
			node->cmd.infile = ft_strdup(tokens[i]);
			if (!isvalidfile(node->cmd.infile))
				return (free_node(node), NULL);
		}
		else if (!strcmp(tok, "<<"))
		{
			if (i + 1 > end - start)
				return (NULL);
			i++;
			if (!isvalidfile(tokens[i]))
				return (free_node(node), NULL);
			if (node->cmd.infile)
				free(node->cmd.infile);
			node->cmd.infile = heredoc(msh, tokens[i]);
			if (!node->cmd.infile)
				return (free_node(node), NULL);
		}
		else if (!strcmp(tok, ">") || !strcmp(tok, ">>"))
		{
			if (i + 1 > end)
				return (NULL);
			i++;
			if (!node->cmd.append && !strcmp(tok, ">>") && !first_out)
			{
				i++;
				continue ;
			}
			node->cmd.outfile = ft_strdup(tokens[i]); // FIXME: Idk what to fix
			if (!isvalidfile(node->cmd.outfile))
				return (free_node(node), NULL);
			node->cmd.append = !strcmp(tok, ">>");
			first_out = true;
		}
		else if (!ft_vector_add(&node->cmd.argv, &tok))
			return (ft_vector_deep_free(node->cmd.argv), free(node), NULL);
		i++;
	}
	tok = NULL;
	if (!ft_vector_add(&node->cmd.argv, &tok))
		return (ft_vector_deep_free(node->cmd.argv), free(node), NULL);
	node->cmd.argc = ft_vector_size(node->cmd.argv) - 1;
	if (node->cmd.argc == 0 && !node->cmd.outfile && !node->cmd.infile)
		return (free_node(node), NULL);
	return (node);
}

static void	apply_out(t_minishell *msh, t_node *node, char *outfile, bool append)
{
	if (node->type == TY_CMD)
	{
		node->cmd.outfile = outfile;
		node->cmd.append = append;
	}
	else if (node->type == TY_PIPE)
	{
		apply_out(msh, node->pipe.right, outfile, append);
	}
	else if (node->type == TY_AND || node->type == TY_OR)
	{
		// Append mode is forced on the right node to simulate the behaviour
		// of bash
		apply_out(msh, node->pipe.left, outfile, append);
		apply_out(msh, node->pipe.right, outfile, true);
	}
}

static void	apply_in(t_minishell *msh, t_node *node, char *infile)
{
	if (node->type == TY_CMD)
	{
		node->cmd.infile = infile;
	}
	else if (node->type == TY_PIPE)
	{
		apply_in(msh, node->pipe.left, infile);
	}
	else if (node->type == TY_AND || node->type == TY_OR)
	{
		apply_in(msh, node->pipe.left, infile);
		apply_in(msh, node->pipe.right, infile);
	}
}

static t_node	*parse_parent(t_minishell *msh, char **tokens, size_t start, size_t end)
{
	char	*outfile = NULL;
	char	*infile = NULL;
	bool	append;
	bool	first_out = true;

	// First find the parenthesis start and end values
	int	parent_start;
	int	parent_end;

	int	open_parents;

	parent_start = - 1;
	parent_end = -1;
	open_parents = 0;
	for (size_t i = start; i <= end; i++)
	{
		if (!strcmp(tokens[i], "("))
		{
			if (parent_start == -1)
				parent_start = i;
			else
				open_parents++;
		}
		else if (!strcmp(tokens[i], ")"))
		{
			if (open_parents == 0 && parent_end == -1)
				parent_end = i;
			else
				open_parents--;
		}
	}

	/* 
	 * FIXME:
	 * /!\ `(ls) (echo)` is accepted by the parser and exec `ls` /!\
	 */

	if (open_parents != 0)
		return (NULL);

	if (parent_start == -1 && parent_end == -1)
		return (parse_cmd(msh, tokens, start, end));
	else if (parent_start == -1 || parent_end == -1)
		return (NULL);

	// Then parse redirections here
	char	*tok;

	// FIXME:
	// Redirection are not allowed before the parenthesis

	append = false;
	for (size_t i = start; i <= end; i++)
	{
		if (i >= (size_t)parent_start && i <= (size_t)parent_end)
			continue ;
		tok = tokens[i];
		if (!strcmp(tok, "<"))
		{
			if (i + 1 > end - start)
				return (NULL);
			i++;
			infile = ft_strdup(tokens[i]); // TODO check heredoc priority
		}
		else if (!strcmp(tok, "<<"))
		{
			if (i + 1 > end - start)
				return (NULL);
			i++;
			infile = heredoc(msh, tokens[i]);
			if (!infile)
				return (NULL); // free outfile, infile if needed
		}
		else if (!strcmp(tok, ">") || !strcmp(tok, ">>"))
		{
			if (i + 1 > end)
				return (NULL);
			i++;
			if (!append && !strcmp(tok, ">>") && !first_out)
				continue ;
			outfile = ft_strdup(tokens[i + start]);
			append = !strcmp(tok, ">>");
			first_out = false;
		}
	}

	// Finally parse what's between parenthesis and add the redirections

	t_node	*node = parse_expr(msh, tokens, parent_start + 1, parent_end - 1);

	if (outfile != NULL)
		apply_out(msh, node, outfile, append);
	if (infile != NULL)
		apply_in(msh, node, infile);

	t_node	*parent = ft_calloc(1, sizeof(t_node));
	parent->type = TY_PARENT;
	parent->parent.node = node;

	return (parent);
}

t_node	*parse_expr(t_minishell *msh, char **tokens, size_t start, size_t end)
{
	int			pos;
	t_node	*node;


	pos = get_op(tokens, start, end);
	if (pos == -1)
		return (parse_parent(msh, tokens, start, end));
	else if (pos == 0)
		return (NULL);
	else if (!strcmp(tokens[pos], "|"))
	{
		node = ft_calloc(1, sizeof(t_node));
		if (!node)
			return (NULL);
		node->type = TY_PIPE;
		node->pipe.left = parse_expr(msh, tokens, start, pos - 1);
		if (!node->pipe.left)
			return (free_node(node), NULL);
		node->pipe.right = parse_expr(msh, tokens, pos + 1, end);
		if (!node->pipe.right)
			return (free_node(node), NULL);
		return (node);
	}
	else if (!strcmp(tokens[pos], "||"))
	{
		node = ft_calloc(1, sizeof(t_node));
		if (!node)
			return (NULL);
		node->type = TY_OR;
		node->pipe.left = parse_expr(msh, tokens, start, pos - 1);
		if (!node->pipe.left)
			return (free_node(node), NULL);
		node->pipe.right = parse_expr(msh, tokens, pos + 1, end);
		if (!node->pipe.right)
			return (free_node(node), NULL);
		return (node);
	}
	else if (!strcmp(tokens[pos], "&&"))
	{
		node = ft_calloc(1, sizeof(t_node));
		if (!node)
			return (NULL);
		node->type = TY_AND;
		node->pipe.left = parse_expr(msh, tokens, start, pos - 1);
		if (!node->pipe.left)
			return (free_node(node), NULL);
		node->pipe.right = parse_expr(msh, tokens, pos + 1, end);
		if (!node->pipe.right)
			return (free_node(node), NULL);
		return (node);
	}
	return (NULL);
}
