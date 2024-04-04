/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 23:38:35 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/04 13:50:52 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>

void	add_our_history(t_minishell *msh, char *line)
{
	int		fd;
	char	buf[256];
	char	*home;

	home = getourenv(msh, "HOME");
	line = ft_strdup(line);
	if (!line)
		return ;
	if (home)
	{
		ft_sprintf(buf, "%s/.msh_history", home);
		fd = open(buf, O_WRONLY | O_APPEND | O_CREAT, 0666);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		close(fd);
	}
	add_history(line);
	ft_vector_add(&msh->history, &line);
}

static void	read_our_history(t_minishell *msh, int fd)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		line[ft_strlen(line) - 1] = '\0';
		ft_vector_add(&msh->history, &line);
		add_history(line);
	}
}

void	load_history(t_minishell *msh)
{
	int		fd;
	char	buf[256];
	char	*home;

	home = getourenv(msh, "HOME");
	msh->history = ft_vector(sizeof(char *), 0);
	if (home)
	{
		ft_sprintf(buf, "%s/.msh_history", home);
		fd = open(buf, O_RDONLY | O_CREAT, 0666);
		if (fd == -1)
		{
			free(home);
			return ;
		}
		read_our_history(msh, fd);
		close(fd);
		free(home);
	}
}

void	free_history(t_minishell *msh)
{
	ft_vector_deep_free(msh->history);
}
