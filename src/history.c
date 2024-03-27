/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 23:38:35 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/28 00:21:07 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/history.h>

void	add_our_history(t_minishell *msh, char *line)
{
	int		fd;
	char	buf[256];
	char	*home;

	home = getourenv(msh, "HOME");
	if (home)
	{
		ft_sprintf(buf, "%s/.msh_history", home);
		fd = open(buf, O_WRONLY | O_APPEND | O_CREAT, 0666);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		close(fd);
	}
	add_history(line);
}

void	load_history(t_minishell *msh)
{
	int		fd;
	char	buf[256];
	char	*home;
	char	*line;

	home = getourenv(msh, "HOME");
	if (home)
	{
		ft_sprintf(buf, "%s/.msh_history", home);
		fd = open(buf, O_RDONLY | O_CREAT, 0666);
		// TODO Maybe store the lines in a vector and free them later ?
		while (1)
		{
			line = get_next_line(fd);
			if (!line)
				break ;
			line[ft_strlen(line) - 1] = '\0';
			add_history(line);
		}
	}
}
