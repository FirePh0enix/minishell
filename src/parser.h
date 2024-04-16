/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:25:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/16 23:44:49 by ledelbec         ###   ########.fr       */
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
	t_type	type;
	union
	{
		struct
		{
			char	**argv;
			int		argc;

			char	*infile;
			char	*outfile;
			bool	append;
		}	cmd;
		struct
		{
			struct s_node	*left;
			struct s_node	*right;
		}	pipe;
		struct
		{
			struct s_node	*node;
		}	parent;
	};
}	t_node;

t_node	*parse_line(t_minishell *minishell, char *line);
void	dump_line(t_node *node);

char	**split_into_tokens(char *line);

t_node	*parse_expr(t_minishell *msh, char **tokens, size_t start, size_t end);
void	free_node(t_node *node);

t_str	expand_str(t_minishell *msh, char *line);
char	**expand_wildcards(char **tokens);

#endif
