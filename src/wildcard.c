/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:05:54 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/12 12:08:43 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

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

typedef struct s_file
{
	char	*file;
	size_t	start;
}	t_file;

static t_file	*filter_files(t_file *files, char *filter)
{
	t_file	*filtered_files;
	char	*prefix;
	char	*suffix;
	char	*suf2;
	size_t	i;
	size_t	i2;

	filtered_files = ft_vector(sizeof(t_file), 0);

	i = 0;
	while (filter[i] && filter[i] != '*')
		i++;
	prefix = strndup(filter, i);

	i2 = i;
	while (filter[i2])
		i2++;
	suffix = strndup(filter + i + 1, i2 - i);

	suf2 = strndup(suffix, ft_strchr(suffix, '*') - suffix);

	//printf("- pre: %s\n", prefix);
	//printf("- suf: %s\n", suffix);
	//printf("- su2: %s\n", suf2);

	i = 0;
	while (i < ft_vector_size(files))
	{
		char	*s3 = strstr(files[i].file + files[i].start, suf2);
		if (starts_with(files[i].file + files[i].start, prefix) && s3)
		{
			t_file	file = { files[i].file, s3 - files[i].file + 1 };
			ft_vector_add(&filtered_files, &file);
		}
		else
		{
			free(files[i].file);
		}
		i++;
	}

	//for (size_t i = 0; i < ft_vector_size(filtered_files); i++)
	//	printf("%s (%s) ", filtered_files[i].file, filtered_files[i].file + filtered_files[i].start);
	//printf("\n\n");

	if (ft_strchr(suffix, '*'))
		filtered_files = filter_files(filtered_files, suffix + 1);
	return (filtered_files);
}

char	**wildcard(char *s, size_t start_idx)
{
	t_file			*files = ft_vector(sizeof(t_file), 0);
	DIR				*dir;
	struct dirent	*dirent;
	char			*s2;
	char			**files3;

	int				i;
	int				i2;

	i = 0;
	while (s[i] && s[i] != '*')
		i++;

	while (i >= 0 && s[i] != '/')
		i--;

	i2 = i + 1;
	while (s[i2] && s[i2] != '/')
		i2++;

	char	*path = strndup(s, i);
	char	*filter = strndup(s + i + 1, i2 - i - 1);

	if (*path == '\0')
		dir = opendir(".");
	else
		dir = opendir(path);
	while (1)
	{
		dirent = readdir(dir);
		if (!dirent)
			break ;
		if (dirent->d_name[0] == '.')
			continue ;
		s2 = ft_strdup(dirent->d_name);
		t_file file = { s2, 0 };
		ft_vector_add(&files, &file);
	}
	closedir(dir);

	if (s[i2] == '\0')
	{
		t_file	*files2 = filter_files(files, filter);
		files3 = ft_vector(sizeof(char *), ft_vector_size(files2));
		for (size_t i = 0; i < ft_vector_size(files2); i++)
		{
			t_str	s = str("");
			str_append(&s, path);
			str_append(&s, "/");
			str_append(&s, files2[i].file);
			ft_vector_add(&files3, &s.data);
		}
	}
	else
	{
		t_file	*files2 = filter_files(files, filter);
		files3 = ft_vector(sizeof(char *), 0);
		for (size_t i = 0; i < ft_vector_size(files2); i++)
		{
			t_str	s2 = str("");
			str_append(&s2, path);
			str_append(&s2, "/");
			str_append(&s2, files2[i].file);
			str_append(&s2, "/");
			str_append(&s2, &s[i2 + 1]);

			char	**files4 = wildcard(s2.data, 0);
			for (size_t i = 0; i < ft_vector_size(files4); i++)
				ft_vector_add(&files3, &files4[i]);
		}
	}
	return (files3);
}
