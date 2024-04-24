/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:21:08 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/24 16:21:45 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	code_for_cmd(char *cmd)
{
	if (cmd == (void *) 1)
		return (126);
	else if (cmd == (void *) 2)
		return (127);
	return (0);
}
