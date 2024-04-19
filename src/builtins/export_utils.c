/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:58:56 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 15:03:49 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_var_name(char *s)
{
	size_t	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (false);
	i++;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	return (s[i] == '\0');
}

int	set_exit_code(int *exit_code, int exit_code2)
{
	if (*exit_code == 0)
		*exit_code = exit_code2;
	return (0);
}

void	sort_str_tab(char *tab[], int size)
{
	char	*swap;
	int		i;
	int		j;

	i = 0;
	while (i < size)
	{
		j = i;
		while (j > 0 && ft_strcmp(tab[j - 1], tab[j]) > 0)
		{
			swap = tab[j];
			tab[j] = tab[j - 1];
			tab[j - 1] = swap;
			j--;
		}
		i++;
	}
}
