/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:54:47 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/29 12:02:47 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <stdbool.h>
# include "parser.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

typedef struct s_minishell	t_minishell;
typedef struct s_node		t_node;

int		code_for_errno(void);
int		code_for_cmd(char *cmd);

void	free_split(char **s);

char	**ft_get_path(t_minishell *msh);
char	*ft_create_path(t_minishell *msh, char *command);
char	*check_access(char *command);

int		exec_cmd(t_minishell *msh, t_node *node, int pipe_in, int pipe_out);
int		ft_exec_cmd(t_minishell *msh, char *cmd, char **av, char **envp);
void	exec_cmd_when_cmd_ok(t_minishell *msh, char *cmd, t_node *node);

int		create_child(t_minishell *msh, t_node *node, int in, int out);
int		wait_for_children(t_minishell *msh);

int		dup_infile(t_node *node);
int		dup_outfile(t_node *node);
int		dup_in(int in);
int		dup_out(int in);
void	overall_dup(t_node *node, int in, int out);

bool	is_builtin(t_node *node);
int		exec_builtin(t_minishell *msh, t_node *node, int in, int out);

void	free_when_no_cmd(t_minishell *msh, t_node *node);
void	free_at_end(t_minishell *msh, t_node *node) __attribute__((noreturn));
void	close_fd_parent(int in, int out);
void	close_fd_child(t_minishell *msh);

int		handle_pipe(t_minishell *msh, t_node *node, int in, int out);
int		handle_or(t_minishell *msh, t_node *node, int in, int out);
int		handle_and(t_minishell *msh, t_node *node, int in, int out);
int		handle_if_not_cmd(t_minishell *msh, t_node *node, int in, int out);

void	add_env(t_minishell *msh, t_node *node);

#endif
