/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:25:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/29 14:27:37 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>

typedef struct s_minishell	t_minishell;

typedef enum e_pretype
{
	PTY_CMD,
	PTY_PIPE,
	PTY_OR,
	PTY_AND,
	PTY_PARENT,
}	t_pretype;

typedef struct s_prenode
{
	t_pretype	type;
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
			struct s_prenode	*left;
			struct s_prenode	*right;
		}	pipe;
		struct
		{
			struct s_prenode	*node;
			char				*infile;
			char				*outfile;
			bool				append;
		}	parent;
	};
}	t_prenode;

typedef enum e_type
{
	TY_CMD,
	TY_PIPE,
	TY_OR,
	TY_AND,
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
	};
}	t_node;

t_node	*parse_line(t_minishell *minishell, char *line);
void	dump_line(t_node *node);

#endif
