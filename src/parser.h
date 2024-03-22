/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:25:25 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/22 20:07:13 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef enum
{
	TY_CMD,
	TY_PIPE,
	TY_RED
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
		}	cmd;
		struct
		{
			struct s_node	*left;
			struct s_node	*right;
		}	pipe;
		struct
		{
			struct s_node	*cmd;
			char			*name;
		}	red;
	};
}	t_node;

t_node	*parse_line(char *line);

#endif
