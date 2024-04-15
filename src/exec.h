/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:54:47 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/15 15:22:36 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <stdbool.h>

typedef struct s_minishell	t_minishell;
typedef struct s_node		t_node;

int		code_for_errno(void);
int		exec_cmd(t_minishell *msh, t_node *node, int pipe_in, int pipe_out);
char	**ft_get_path(t_minishell *msh);
char	*ft_create_path(t_minishell *msh, char *command);
int		ft_exec_cmd(char *cmd, char **av, char **envp);

int		dup_infile(t_node *node);
int		dup_outfile(t_node *node);
int		dup_in(int	in);
void	overall_dup(t_node *node, int in, int out);
int		dup_out(int	in);
void	overall_dup(t_node *node, int in, int out);

bool	is_builtin(t_node *node);
int		exec_builtin(t_minishell *msh, t_node *node, int in, int out);

void	close_fd_parent(int in, int out);
void	close_fd_child(t_minishell *msh);

int		handle_pipe(t_minishell *msh, t_node *node, int in, int out);
int		handle_or(t_minishell *msh, t_node *node, int in, int out);
int		handle_and(t_minishell *msh, t_node *node, int in, int out);
int		handle_if_not_cmd(t_minishell *msh, t_node *node, int in, int out);

int		wait_for_children(t_minishell *msh);

#endif
