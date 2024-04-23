/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:05:54 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/23 14:59:32 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

t_tok			*wildcard(char *s);

static void	recurse_wildcard2(t_str *s2, t_tok **files3, char *file,
	char *suffix)
{
	size_t	i2;
	t_tok	*files4;

	str_append(s2, file);
	str_append(s2, "/");
	str_append(s2, suffix);
	files4 = wildcard(s2->data);
	i2 = 0;
	while (i2 < ft_vector_size(files4))
		ft_vector_add(files3, &files4[i2++]);
	ft_vector_free(files4);
}

static void	add_file(t_str *s2, t_tok **files3, char *file)
{
	t_tok	tk;

	str_append(s2, file);
	tk = tok(TOK_IDENT, s2->data);
	ft_vector_add(files3, &tk);
}

void	free_files(t_file *files);

static t_tok	*wildcard2(char *wc, char *filter, t_file *files, char *path)
{
	t_file	*files2;
	t_tok	*files3;
	size_t	i;
	t_str	s2;

	files2 = filter_files(files, ft_strdup(filter));
	files3 = ft_vector(sizeof(t_tok), 0);
	i = 0;
	while (i < ft_vector_size(files2))
	{
		s2 = str("");
		if (*path != '.')
		{
			str_append(&s2, path);
			str_append(&s2, "/");
		}
		if (wc[0] != '\0')
			recurse_wildcard2(&s2, &files3, files2[i].file, &wc[1]);
		else
			add_file(&s2, &files3, files2[i].file);
		i++;
	}
	free_files(files2);
	return (free(path), free(filter), files3);
}

static t_file	*read_directory(char *path)
{
	DIR				*dir;
	struct dirent	*dirent;
	t_file			*files;
	char			*s2;
	t_file			file;

	dir = opendir(path);
	if (!dir)
		return (ft_vector(sizeof(t_tok), 0));
	files = ft_vector(sizeof(t_file), 0);
	while (1)
	{
		dirent = readdir(dir);
		if (!dirent)
			break ;
		if (dirent->d_name[0] == '.')
			continue ;
		s2 = ft_strdup(dirent->d_name);
		file = (t_file){s2, 0};
		ft_vector_add(&files, &file);
	}
	closedir(dir);
	return (files);
}

t_tok	*wildcard(char *s)
{
	int				i;
	int				i2;
	char			*path;
	char			*filter;

	i = 0;
	while (s[i] && s[i] != '*')
		i++;
	while (i >= 0 && s[i] != '/')
		i--;
	i2 = i + 1;
	while (s[i2] && s[i2] != '/')
		i2++;
	if (i >= 0 && s[i] == '/')
		path = ft_strndup(s, i);
	else
		path = ft_strdup("");
	filter = ft_strndup(s + i + 1, i2 - i - 1);
	if (*path == '\0')
	{
		free(path);
		path = ft_strdup(".");
	}
	return (wildcard2(&s[i2], filter, read_directory(path), path));
}
