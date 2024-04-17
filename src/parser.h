/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:25:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/17 20:20:31 by ledelbec         ###   ########.fr       */
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

			char	*infile;
			char	*outfile;
			bool	append;

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
	TOK_STR,
	TOK_OP
}	t_tok_type;

typedef struct s_tok
{
	t_tok_type	type;
	char	*s;
}	t_tok;

inline t_tok	nulltok()
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

t_node	*parse_expr(t_minishell *msh, t_tok *tokens, size_t start, size_t end, t_node *parent);
void	free_node(t_node *node);

t_str	expand_str(t_minishell *msh, char *line);
t_tok	*expand_wildcards(t_tok *tokens);

#endif
