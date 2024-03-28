/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:50:42 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/28 13:45:57 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	msh_error(char *msg)
{
	write(2, "msh: ", 5);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	msh_error_cmd(char *cmd)
{
	write(2, "msh: cannot find command `", 26);
	write(2, cmd, ft_strlen(cmd));
	write(2, "`\n", 2);
}
