/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:25:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/28 12:20:42 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include <stddef.h>
# include "libft.h"

typedef struct s_minishell	t_minishell;

typedef enum e_type
{
	TY_CMD,
	TY_PIPE,
	TY_OR,
	TY_AND,
	TY_PARENT,
}	t_type;

typedef enum e_red_type
{
	RED_IN,
	RED_OUT,
}	t_red_type;

typedef struct s_red
{
	char		*filename;
	t_red_type	type;
	bool		append;
}	t_red;

typedef struct s_node
{
	t_type			type;
	struct s_node	*parent;
	union
	{
		struct s_cmd
		{
			char	**argv;
			int		argc;

			t_red	*all_reds;

			char	*infile;
			char	*outfile;
			bool	append;
			bool	heredoc;

			char	**env;
		}	cmd;
		struct s_pipe
		{
			struct s_node	*left;
			struct s_node	*right;
		}	pipe;
		struct s_parent
		{
			struct s_node	*node;
		}	pa;
	};
}	t_node;

typedef enum e_tok_type
{
	TOK_IDENT,
	TOK_STR,
	TOK_OP
}	t_tok_type;

typedef struct s_tok
{
	t_tok_type	type;
	char		*s;
}	t_tok;

typedef struct s_range
{
	size_t	start;
	size_t	end;
}	t_range;

inline t_range	range(size_t start, size_t end)
{
	return ((t_range){start, end});
}

inline t_tok	nulltok(void)
{
	return ((t_tok){.type = 0, .s = NULL});
}

inline t_tok	tok(t_tok_type type, char *s)
{
	return ((t_tok){.type = type, .s = s});
}

t_node	*parse_line(t_minishell *minishell, char *line);
void	dump_line(t_node *node);

t_tok	*split_into_tokens(char *line);
void	free_tokens(t_tok *tokens);

t_node	*parse_expr(t_tok *tokens, t_range range, t_node *parent, int pos);
void	free_node(t_node *node);
void	free_node_in_child(t_node *node);
void	free_node_tree(t_node *node);

t_str	expand_str(t_minishell *msh, char *line);
void	expand_env(t_minishell *msh, t_str *s, char *line, size_t *index);

bool	can_expand_env(char *line, size_t i);

t_tok	*expand_wildcards(t_tok *tokens);
t_tok	*cleanup_tokens(t_tok *tokens);

bool	is_single_op(char c);
bool	is_dual_op(char *line);

bool	is_valid_env_ch(char c);

#endif
