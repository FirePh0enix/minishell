/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:18:23 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/20 16:28:46 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static t_tok	clean_token(t_tok tk)
{
	t_str	s;
	size_t	i;

	s = str("");
	i = 0;
	while (tk.s[i])
	{
		if (tk.s[i] == '\\' && tk.s[i + 1] == '*')
		{
			str_append(&s, "*");
			i++;
		}
		else
			str_append_n(&s, &tk.s[i], 1);
		i++;
	}
	free(tk.s);
	return (tok(tk.type, s.data));
}

t_tok	*cleanup_tokens(t_tok *tokens)
{
	size_t	i;

	i = 0;
	while (i < ft_vector_size(tokens))
	{
		tokens[i] = clean_token(tokens[i]);
		i++;
	}
	return (tokens);
}
