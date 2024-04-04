/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:54:47 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/04 18:18:57 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

typedef struct s_minishell	t_minishell;
typedef struct s_node		t_node;

int		exec_cmd(t_minishell *msh, t_node *node, int pipe_in, int pipe_out, int child);
char	**ft_get_path(t_minishell *msh);
char	*ft_create_path(t_minishell *msh, char *command);
int		ft_exec_cmd(char *cmd, char **av, char **envp);
#endif