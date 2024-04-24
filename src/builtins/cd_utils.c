/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:48:28 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/24 16:57:42 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_chdir(char *path)
{
	msh_errno2("cd", path);
	free(path);
	return (1);
}

void	free_cd(char *fs, char *cdpath)
{
	free(fs);
	if (cdpath)
		free(cdpath);	
}
