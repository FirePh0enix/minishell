/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:34:59 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 13:19:08 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include <string.h>
#include <readline/readline.h>

static int	get_op_priority(char *s)
{
	if (!strcmp(s, "|"))
		return (1);
	else if (!strcmp(s, "||") || !strcmp(s, "&&"))
		return (2);
	return (-1);
}

static int	get_op(t_tok *tokens, size_t start, size_t end)
{
	int		i;
	char	*tok;
	int		hprio;
	int		pos;
	int		parent;

	pos = -1;
	i = start;
	hprio = 0;
	parent = 0;
	while (i <= (int) end)
	{
		tok = tokens[i].s;
		if (tokens[i].type == TOK_OP && !strcmp(tok, ")"))
			parent--;
		else if (tokens[i].type == TOK_OP && !strcmp(tok, "("))
			parent++;
		else if (parent == 0 && tokens[i].type == TOK_OP && get_op_priority(tok) > hprio)
		{
			hprio = get_op_priority(tok);
			pos = i;
		}
		i++;
	}
	return (pos);
}

static char	*heredoc(t_minishell *msh, char *eof)
{
	char		*line;
	int			fd;
	char		filename[256];
	int			line_num;

	ft_sprintf(filename, "/tmp/msh-miniseashell-heredoc-%zu", msh->heredocs++);
	// TODO: Maybe increment files based on which one are available, to allow
	//       multiple minishell running at the same tome.
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		return (NULL);
	line_num = 1;
	set_ctrlc_heredoc();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_fprintf(2, "msh: warning: here-document delimited at line %d by "
				"end-of-file  (wanted `%s')\n", line_num, eof);
			break ;
		}
		else if (!strcmp(line, eof))
			break ;
		if (g_signum == SIGINT)
			break ;
		ft_putendl_fd(line, fd);
		free(line);
		line_num++;
	}
	set_ctrlc_default();
	return (close(fd), ft_strdup(filename));
}

static bool	isvalidfile(char *s)
{
	return (!(!strcmp(s, "<") || !strcmp(s, ">") || !strcmp(s, "<<")
		|| !strcmp(s, ">>") || !strcmp(s, "|") || !strcmp(s, "||")
		|| !strcmp(s, "&") || !strcmp(s, "&&")));
}

int	open_redirect(char *filename, char *redirect)
{
	int	fd;

	if (!strcmp(redirect, ">"))
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (!strcmp(redirect, ">>"))
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
	else if (!strcmp(redirect, "<") || !strcmp(redirect, "<<"))
		fd = open(filename, O_RDONLY);
	else
		fd = -1;
	if (fd != -1)
		close(fd);
	else
		msh_errno(filename);
	return (fd);
}

static int	handle_redirects(t_minishell *msh, t_node *node, t_tok *tokens,
	size_t i)
{
	const size_t	size = ft_vector_size(tokens);
	char			*s;

	if (i + 1 >= size)
		return (-1);
	if (!strcmp(tokens[i].s, ">") || !strcmp(tokens[i].s, ">>"))
	{
		if (!isvalidfile(tokens[i + 1].s))
			return (-1);
		open_redirect(tokens[i + 1].s, tokens[i].s);
		node->cmd.outfile = ft_strdup(tokens[i + 1].s);
		node->cmd.append = !strcmp(tokens[i].s, ">>");
	}
	else if (!strcmp(tokens[i].s, "<"))
	{
		if (!isvalidfile(tokens[i + 1].s))
			return (-1);
		if (open_redirect(tokens[i + 1].s, tokens[i].s) == -1)
			return (-1);
		node->cmd.infile = ft_strdup(tokens[i + 1].s);
	}
	else if (!strcmp(tokens[i].s, "<<"))
	{
		s = heredoc(msh, tokens[i + 1].s);
		open_redirect(s, tokens[i].s);
		node->cmd.infile = s;
	}
	return (0);
}

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
		s2 = strndup(tokens[i].s, s - tokens[i].s);
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

static t_node	*parse_cmd(t_minishell *msh, t_tok *tokens,
		size_t start, size_t end, t_node *parent)
{
	t_node	*node;
	size_t	i;
	char	*tok;

	node = ft_calloc(sizeof(t_node), 1);
	if (!node)
		return (NULL);
	node->type = TY_CMD;
	node->parent = parent;
	node->cmd.argv = ft_vector(sizeof(char *), 0);
	node->cmd.env = ft_vector(sizeof(char *), 0);
	if (!node->cmd.argv || !node->cmd.env)
		return (free(node), NULL);
	i = start;
	read_vars(node, tokens, &i, end);
	while (i <= end)
	{
		tok = ft_strdup(tokens[i].s);
		if (tokens[i].type == TOK_OP
			&& (!strcmp(tok, ">") || !strcmp(tok, ">>") || !strcmp(tok, "<")
				|| !strcmp(tok, "<<")))
		{
			if (handle_redirects(msh, node, tokens, i) == -1)
				return ((void *)1);
			i++;
		}
		else if (!ft_vector_add(&node->cmd.argv, &tok))
			return (ft_vector_deep_free(node->cmd.argv), free(node), NULL);
		i++;
	}
	tok = NULL;
	if (!ft_vector_add(&node->cmd.argv, &tok))
		return (ft_vector_deep_free(node->cmd.argv), free(node), NULL);
	node->cmd.argc = ft_vector_size(node->cmd.argv) - 1;
	if (node->cmd.argc == 0 && !node->cmd.outfile && !node->cmd.infile && ft_vector_size(node->cmd.env) == 0)
		return (free_node(node), NULL);
	return (node);
}

static void	apply_out(t_minishell *msh, t_node *node, char *outfile, bool append)
{
	if (node->type == TY_CMD)
	{
		node->cmd.outfile = ft_strdup(outfile);
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
		node->cmd.infile = ft_strdup(infile);
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

static t_node	*parse_parent(t_minishell *msh, t_tok *tokens, size_t start, size_t end, t_node *parent)
{
	// First find the parenthesis start and end values
	int	parent_start;
	int	parent_end;

	int	open_parents;

	parent_start = - 1;
	parent_end = -1;
	open_parents = 0;
	for (size_t i = start; i <= end; i++)
	{
		if (tokens[i].type == TOK_OP && !strcmp(tokens[i].s, "("))
		{
			if (parent_start == -1)
				parent_start = i;
			else
				open_parents++;
		}
		else if (tokens[i].type == TOK_OP && !strcmp(tokens[i].s, ")"))
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
		return (parse_cmd(msh, tokens, start, end, parent));
	else if (parent_start == -1 || parent_end == -1)
		return (NULL);

	if (parent_start != (int) start)
		return (NULL);

	size_t	i;
	t_node	n;
	char	*tok;

	ft_bzero(&n, sizeof(t_node));
	i = start;
	while (i <= end)
	{
		if ((int)i >= parent_start && (int)i <= parent_end)
		{
			i++;
			continue ;
		}
		tok = tokens[i].s;
		if (tokens[i].type == TOK_OP
			&& (!strcmp(tok, ">") || !strcmp(tok, ">>") || !strcmp(tok, "<")
				|| !strcmp(tok, "<<")))
		{
			if (handle_redirects(msh, &n, tokens, i) == -1)
				return ((void *) 1);
			i++;
		}
		else
			return (NULL);
		i++;
	}

	// Finally parse what's between parenthesis and add the redirections

	t_node	*node = parse_expr(msh, tokens, parent_start + 1, parent_end - 1, NULL);
	if (!node)
		return (NULL);

	if (n.cmd.outfile != NULL)
		apply_out(msh, node, n.cmd.outfile, n.cmd.append);
	if (n.cmd.infile != NULL)
		apply_in(msh, node, n.cmd.infile);

	t_node	*pa = ft_calloc(1, sizeof(t_node));
	if (!pa)
		return (NULL);
	pa->type = TY_PARENT;
	pa->parent = parent;
	pa->pa.node = node;
	node->parent = pa;

	return (pa);
}

static t_type	type_for_str(char *s)
{
	if (!strcmp(s, "|"))
		return (TY_PIPE);
	else if (!strcmp(s, "||"))
		return (TY_OR);
	else if (!strcmp(s, "&&"))
		return (TY_AND);
	return (0);
}

t_node	*parse_expr(t_minishell *msh, t_tok *tokens, size_t start, size_t end,
	t_node *parent)
{
	int			pos;
	t_node	*node;

	pos = get_op(tokens, start, end);
	if (pos == -1)
		return (parse_parent(msh, tokens, start, end, parent));
	else if (pos == 0)
	{
		printf("3.\n");
		return (NULL);
	}
	else if (!strcmp(tokens[pos].s, "|") || !strcmp(tokens[pos].s, "||")
			|| !strcmp(tokens[pos].s, "&&"))
	{
		node = ft_calloc(1, sizeof(t_node));
		if (!node)
			return (NULL);
		node->type = type_for_str(tokens[pos].s);
		node->parent = parent;
		node->pipe.left = parse_expr(msh, tokens, start, pos - 1, node);
		if (!node->pipe.left)
			return (free_node(node), NULL);
		node->pipe.right = parse_expr(msh, tokens, pos + 1, end, node);
		if (!node->pipe.right)
			return (free_node(node), NULL);
		return (node);
	}
	return (NULL);
}
