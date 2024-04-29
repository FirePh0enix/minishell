/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:21:41 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/29 11:18:01 by ledelbec         ###   ########.fr       */
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
			ft_sprintf(buf, fhome_err(msh), msh->exit_code, cwd + sz);
		else
			ft_sprintf(buf, fhome(msh), 0, cwd + sz);
	}
	else
	{
		if (msh->exit_code != 0)
			ft_sprintf(buf, fnohome_err(msh), msh->exit_code, cwd_or(cwd));
		else
			ft_sprintf(buf, fnohome(msh), msh->exit_code, cwd_or(cwd));
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
			ft_sprintf(buf, fnohome(msh), 0, cwd_or(cwd));
		else
			ft_sprintf(buf, fnohome_err(msh), msh->exit_code, cwd_or(cwd));
		free(cwd);
		return ;
	}
	write_prefix2(msh, buf, home, cwd);
	free(home);
	free(cwd);
}
