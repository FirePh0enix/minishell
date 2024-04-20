/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:13:03 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/20 16:38:32 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"
#include "../minishell.h"

void	append_array(t_tok **tokens2, t_tok *tokens3)
{
	size_t	i2;

	i2 = 0;
	while (i2 < ft_vector_size(tokens3))
		ft_vector_add(tokens2, &tokens3[i2++]);
}

t_tok	*expand_wildcards2(t_tok *tokens, t_tok *tokens2, size_t i)
{
	char	*s;
	t_tok	*tokens3;
	size_t	idx;

	i = 0;
	while (i < ft_vector_size(tokens))
	{
		s = ft_strchr(tokens[i].s, '*');
		idx = s - tokens[i].s;
		if (s && (idx == 0 || s[-1] != '\\'))
		{
			tokens3 = wildcard(tokens[i].s);
			if (!tokens3)
				return (NULL);
			if (ft_vector_size(tokens3) == 0)
				ft_vector_add(&tokens2, &tokens[i]);
			else
				append_array(&tokens2, tokens3);
		}
		else
			ft_vector_add(&tokens2, &tokens[i]);
		i++;
	}
	return (tokens2);
}

t_tok	*expand_wildcards(t_tok *tokens)
{
	t_tok	*tokens2;

	tokens2 = ft_vector(sizeof(t_tok), 0);
	if (!tokens2)
		return (NULL);
	return (expand_wildcards2(tokens, tokens2, 0));
}
