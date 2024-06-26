/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:06:09 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/25 12:13:17 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>

static bool	starts_with(char *s, char *start)
{
	const size_t	size = ft_strlen(s);
	const size_t	start_size = ft_strlen(start);

	return (start_size == 0
		|| (size >= start_size && !ft_strncmp(s, start, start_size)));
}

static t_file	*filter_recurse(t_file *filtered_files, char *suffix)
{
	if (ft_strchr(suffix, '*'))
		return (filter_files(filtered_files, ft_strdup(suffix + 1)));
	else
		return (filtered_files);
}

static void	add_file_if_valid(t_file *files, t_file **filtered_files,
	char *prefix, char *suffix)
{
	size_t	i;
	char	*suf2;
	char	*s3;
	t_file	file;
	size_t	nstart;

	if (ft_strchr(suffix, '*'))
		suf2 = ft_strndup(suffix, ft_strchr(suffix, '*') - suffix);
	else
		suf2 = ft_strdup(suffix);
	i = 0;
	while (i < ft_vector_size(files))
	{
		s3 = ft_strstr(files[i].file + files[i].start, suf2);
		if (starts_with(files[i].file + files[i].start, prefix) && s3)
		{
			nstart = s3 - files[i].file + 1;
			if (nstart >= ft_strlen(files[i].file))
				nstart = ft_strlen(files[i].file) - 1;
			file = (t_file){ft_strdup(files[i].file), nstart};
			ft_vector_add(filtered_files, &file);
		}
		i++;
	}
	free(suf2);
}

void	free_files(t_file *files)
{
	size_t	i;

	i = 0;
	while (i < ft_vector_size(files))
	{
		free(files[i].file);
		i++;
	}
	ft_vector_free(files);
}

t_file	*filter_files(t_file *files, char *filter)
{
	t_file	*filtered_files;
	char	*prefix;
	char	*suffix;
	size_t	i;
	size_t	i2;

	filtered_files = ft_vector(sizeof(t_file), 0);
	i = 0;
	while (filter[i] && filter[i] != '*')
		i++;
	prefix = ft_strndup(filter, i);
	i2 = i;
	while (filter[i2])
		i2++;
	if (filter[i] != '\0')
		suffix = ft_strndup(filter + i + 1, i2 - i);
	else
		suffix = ft_strndup(filter + i, i2 - i);
	add_file_if_valid(files, &filtered_files, prefix, suffix);
	filtered_files = filter_recurse(filtered_files, suffix);
	free(prefix);
	free(suffix);
	free(filter);
	free_files(files);
	return (filtered_files);
}
