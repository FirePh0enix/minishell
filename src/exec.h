/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:54:47 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/26 13:29:46 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

typedef struct s_minishell	t_minishell;
typedef struct s_node		t_node;

int	exec_cmd(t_minishell *msh, t_node *node, int pipe_in, int pipe_out);

#endif