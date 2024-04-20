/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:41:56 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/20 17:51:59 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/readline.h>
#include <unistd.h>
#include <limits.h>

static int	open_heredoc_inc(char *filename)
{
	int		n;

	n = 0;
	while (n < INT_MAX - 1)
	{
		ft_sprintf(filename, "/tmp/msh-miniseashell-heredoc-%zu", n);
		if (access(filename, R_OK) == -1)
			return (open(filename, O_WRONLY | O_CREAT | O_TRUNC));
		n++;
	}
	return (-1);
}

static void	print_and_free(char *line, int fd)
{
	ft_putendl_fd(line, fd);
	free(line);
}

#define A "msh: warning: here-document delimited at line %d by "
#define B "end-of-file  (wanted `%s')\n"

char	*heredoc(t_minishell *msh, char *eof)
{
	char		*line;
	int			fd;
	char		filename[256];
	int			line_num;

	(void) msh;
	fd = open_heredoc_inc(filename);
	if (fd == -1)
		return (NULL);
	line_num = 1;
	set_ctrlc_heredoc();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_fprintf(2, A B, line_num, eof);
			break ;
		}
		else if (!ft_strcmp(line, eof) || g_signum == SIGINT)
			break ;
		print_and_free(line, fd);
		line_num++;
	}
	return (set_ctrlc_default(), close(fd), ft_strdup(filename));
}
