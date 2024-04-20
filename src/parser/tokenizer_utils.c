/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:15:29 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/20 11:15:43 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

bool	is_single_op(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '(' || c == ')');
}

bool	is_dual_op(char *line)
{
	return (!ft_strncmp(line, "<<", 2) || !ft_strncmp(line, ">>", 2)
		|| !ft_strncmp(line, "&&", 2) || !ft_strncmp(line, "||", 2));
}
