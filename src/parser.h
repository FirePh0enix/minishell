/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:25:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/26 16:16:44 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_minishell	t_minishell;

typedef enum e_type
{
	TY_CMD,
	TY_PIPE,
	TY_RED
}	t_type;

typedef enum e_dir
{
	IN,
	OUT,
	OUT_APPEND
}	t_dir;

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
