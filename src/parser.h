/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:25:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/04 13:37:14 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include <stddef.h>

typedef struct s_minishell	t_minishell;

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

char	*next_string(char *line, size_t *index);
char	*next_token(char *line, size_t *index);
char	**split_into_tokens(char *line);

t_node	*parse_expr(t_minishell *msh, char **tokens, size_t start, size_t end);

#endif
