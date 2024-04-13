/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:24:39 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/13 22:42:11 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "parser.h"
# include "exec.h"
# include <sys/types.h>
# include <sys/uio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <stdio.h>
# include <errno.h>

# define COL_LBLUE  "\e[0;34m"
# define COL_LGREEN "\e[1;32m"
# define COL_RESET  "\e[0m"

typedef struct s_minishell
{
	int		exit_code;
	char	**env;
	char	**history;
	size_t	heredocs;

	int		*open_fds;
}	t_minishell;

extern int	g_signum;

void	prompt(t_minishell *minishell);
void	print_prompt_prefix();
void	init_signals(t_minishell *minishell);

void	add_our_history(t_minishell *msh, char *line);
void	load_history(t_minishell *msh);
void	free_history(t_minishell *msh);

char	**wildcard(char *suffix);

void	copy_env(t_minishell *minishell, char *envp[]);
char	*getourenv(t_minishell *minishell, char *name);
void	setourenv(t_minishell *msh, char *name, char *value);
void	free_env(t_minishell *msh);

void	msh_error(char *msg);
void	msh_builtin_error(char *builtin, char *msg);
void	msh_error_cmd(char *cmd);
void	msh_errno(char *cmd);

int		builtin_cd(t_minishell *msh, int ac, char **av, int parent_in, int parent_out, t_node *node);
int		builtin_pwd(int ac, char **av, int parent_out, t_node *node);
int		builtin_echo(int ac, char **av, int parent_out, t_node *node);
int		builtin_exit(int ac, char **av, int parent_in, int parent_out, t_node *node);
int		builtin_unset(t_minishell *msh, int ac, char **av, t_node *node);
int		builtin_env(t_minishell *msh, int parent_in, int parent_out, t_node *node);
int		builtin_export(t_minishell *msh, int ac, char *av[], int in, int out, t_node *node);

#endif
