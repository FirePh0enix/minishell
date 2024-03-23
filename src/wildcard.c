/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:05:54 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/23 17:43:36 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

static int	ends_with(char *s, char *suffix)
{
	const size_t	size = ft_strlen(s);
	const size_t	ssuf = ft_strlen(suffix);

	return (size >= ssuf && ft_strncmp(s + size - ssuf, suffix, ssuf) == 0);
}

char	**wildcard(char *suffix)
{
	char			cwd[256];
	DIR				*dir;
	char			**files;
	struct dirent	*dirent;
	char			*s;

	getcwd(cwd, 256);
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
		else if (suffix && !ends_with(dirent->d_name, suffix))
			continue ;
		s = ft_strdup(dirent->d_name);
		ft_vector_add(&files, &s);
	}
	closedir(dir);
	return (files);
}
