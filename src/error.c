/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:50:42 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/24 16:33:40 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "libft.h"

void	msh_error(char *msg)
{
	write(2, "msh: ", 5);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	msh_builtin_error(char *builtin, char *msg)
{
	ft_putstr_fd(builtin, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}

void	msh_error_cmd(char *cmd)
{
	write(2, "msh: cannot find command `", 26);
	write(2, cmd, ft_strlen(cmd));
	write(2, "`\n", 2);
}

void	msh_errno(char *cmd)
{
	if (*cmd != '\0')
		ft_fprintf(2, "msh: %s: %s\n", cmd, strerror(errno));
	else
		ft_fprintf(2, "msh: %s\n", strerror(errno));
}

void	msh_errno2(char *cmd, char *msg)
{
	ft_fprintf(2, "msh: %s: %s: %s\n", cmd, msg, strerror(errno));
}
