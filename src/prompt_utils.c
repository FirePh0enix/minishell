/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:21:41 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 14:22:41 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "minishell.h"

static char	*cwd_or(char *s)
{
	if (!s)
		return ("???");
	return (s);
}

static void	write_prefix2(t_minishell *msh, char buf[], char *home, char *cwd)
{
	size_t	sz;
	size_t	cwd_sz;

	sz = ft_strlen(home);
	if (cwd)
		cwd_sz = ft_strlen(cwd);
	else
		cwd_sz = 0;
	if (cwd && cwd_sz >= sz && !ft_strncmp(home, cwd, sz))
	{
		if (msh->exit_code != 0)
			ft_sprintf(buf, HOME_ERR, msh->exit_code, cwd + sz);
		else
			ft_sprintf(buf, HOME, 0, cwd + sz);
	}
	else
	{
		if (msh->exit_code != 0)
			ft_sprintf(buf, NOHOME_ERR, msh->exit_code, cwd_or(cwd));
		else
			ft_sprintf(buf, NOHOME, msh->exit_code, cwd_or(cwd));
	}
}

void	write_prefix(t_minishell *msh, char buf[])
{
	char	*cwd;
	char	*home;

	cwd = getcwd(NULL, 0);
	home = getourenv(msh, "HOME");
	if (!home)
	{
		if (msh->exit_code == 0)
			ft_sprintf(buf, NOHOME, 0, cwd_or(cwd));
		else
			ft_sprintf(buf, NOHOME_ERR, msh->exit_code, cwd_or(cwd));
		return ;
	}
	write_prefix2(msh, buf, home, cwd);
	free(home);
}
