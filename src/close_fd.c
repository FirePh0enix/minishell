/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:27:11 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/13 19:37:37 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void	close_fd_parent(int in, int out)
{
	if (in != -1)
		close(in);
	if (out != -1)
		close(out);
	return ;
}

void	close_fd_child(t_minishell *msh)
{
	int	i;
	int	nb_fd;

	i = 0;
	nb_fd = ft_vector_size(msh->open_fds);
	while (i < nb_fd)
	{
		close(msh->open_fds[i]);
		i++;
	}
	ft_vector_free(msh->open_fds);
}
