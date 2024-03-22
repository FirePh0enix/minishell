/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:05:54 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/22 23:22:11 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

char	**wildcard()
{
	char			cwd[64];
	DIR				*dir;
	char			**files;
	struct dirent	*dirent;
	char			*s;

	getcwd(cwd, 64);
	dir = opendir(cwd);
	if (!dir)
		return (NULL);
	files = ft_vector(sizeof(char *), 0);
	while (1)
	{
		dirent = readdir(dir);
		if (!dirent)
			break ;
		if (dirent->d_name[0] == '.')
			continue ;
		s = ft_strdup(dirent->d_name);
		ft_vector_add(&files, &s);
	}
	closedir(dir);
	return (files);
}
