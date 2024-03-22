/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:22:50 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/22 16:46:32 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>
#include "libft.h"

void	prompt()
{
	char	*line;
	char	buffer[64];
	char	cwd[64];

	while (1)
	{
		getcwd(cwd, 64);
		ft_sprintf(buffer, "%s $ ", cwd);
		line = readline(buffer);
	}
}
