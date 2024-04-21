/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 13:16:52 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/21 13:12:10 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static void	handle_quotes(char *line, size_t *index)
{
	size_t	i;

	i = *index;
	if (line[i] == '"')
	{
		i--;
		while (i >= 2 && line[i] != '"')
			i--;
	}
	else if (line[i] == '\'')
	{
		i--;
		while (i >= 2 && line[i] != '\'')
			i--;
	}
	*index = i;
}

bool	is_just_after_heredoc(char *line, size_t i)
{
	if (i < 2)
		return (false);
	i--;
	while (i >= 2)
	{
		if (ft_strlen(&line[i]) >= 2 && (!ft_strncmp(line, "&&", 2)
				|| !ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "||", 2)))
			break ;
		else if (ft_strlen(&line[i]) >= 2 && !ft_strncmp(line, "<<", 2))
			return (true);
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			break ;
		handle_quotes(line, &i);
		i--;
	}
	return (false);
}
