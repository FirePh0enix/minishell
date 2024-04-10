/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:05:54 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/11 00:26:41 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

/*static int	ends_with(char *s, char *suffix)
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
}*/

static bool	starts_with(char *s, char *start)
{
	const size_t	size = ft_strlen(s);
	const size_t	start_size = ft_strlen(start);

	return (start_size == 0 || (size >= start_size && !ft_strncmp(s, start, start_size)));
}

static bool	ends_with(char *s, char *end)
{
	const size_t	size = ft_strlen(s);
	const size_t	end_size = ft_strlen(end);

	return (end_size == 0 || (size >= end_size && !ft_strncmp(s + size - end_size, end, end_size)));
}

char	**wildcard(char *s)
{
	int				i;
	int				i2;
	char			**res;

	char			*path;

	DIR				*dir;
	struct dirent	*dirent;

	char			*prefix;
	char			*suffix;

	t_str			s2;

	i = 0;
	while (s[i] && s[i] != '*')
		i++;

	i2 = i;
	while (i2 >= 0 && s[i2] != '/')
		i2--;

	prefix = strndup(s + i2 + 1, i - i2 - 1);
	path = strndup(s, i2 + 1);

	i2 = i;
	while (s[i2] && s[i2] != '/')
		i2++;

	suffix = strndup(s + i + 1, i2 - i - 1);
	res = ft_vector(sizeof(char *), 0);

	if (s[i2] == '\0')
	{
		if (*path == '\0')
		{
			free(path);
			path = ft_strdup(".");
		}
		dir = opendir(path);
		while (1)
		{
			dirent = readdir(dir);
			if (!dirent)
				break ;
			if (dirent->d_name[0] == '.')
				continue ;
			if (!starts_with(dirent->d_name, prefix) || !ends_with(dirent->d_name, suffix))
				continue ;
			s2 = str("");
			str_append(&s2, path);
			str_append(&s2, dirent->d_name);
			ft_vector_add(&res, &s2.data);
		}
		closedir(dir);
	}
	else
	{
		char	*past = ft_strdup(s + i2 + 1);
		int		fd;

		dir = opendir(path);
		while (1)
		{
			dirent = readdir(dir);
			if (!dirent)
				break ;
			if (dirent->d_name[0] == '.')
				continue ;
			if (!starts_with(dirent->d_name, prefix) || !ends_with(dirent->d_name, suffix))
				continue ;
			s2 = str("");
			str_append(&s2, path);
			str_append(&s2, dirent->d_name);
			fd = open(s2.data, O_DIRECTORY | O_RDONLY);
			if (fd == -1)
			{
				str_free(&s2);
				continue ;
			}
			close(fd);
			str_append(&s2, "/");
			str_append(&s2, past);
			
			char	**res2 = wildcard(s2.data);
			for (size_t i3 = 0; i3 < ft_vector_size(res2); i3++)
				ft_vector_add(&res, &res2[i3]);
			ft_vector_free(res2);
		}
		closedir(dir);
		free(past);
	}

	free(path);

	return (res);
}
