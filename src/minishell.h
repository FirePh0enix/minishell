/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:24:39 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/28 15:17:27 by ledelbec         ###   ########.fr       */
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
	int		pipe[2];
	int		exit_code;
	char	**env;
	size_t	heredocs;
}	t_minishell;

extern int	g_signum;

void	prompt(t_minishell *minishell);
void	print_prompt_prefix();
void	init_signals(t_minishell *minishell);

void	add_our_history(t_minishell *msh, char *line);
void	load_history(t_minishell *msh);

char	**wildcard(char *suffix);

void	copy_env(t_minishell *minishell, char *envp[]);
char	*getourenv(t_minishell *minishell, char *name);
void	setourenv(t_minishell *msh, char *name, char *value);

void	msh_error(char *msg);
void	msh_error_cmd(char *cmd);

int		builtin_cd(t_minishell *msh, int ac, char **av);
int		builtin_pwd(int ac, char **av);
int		builtin_echo(int ac, char **av);
int		builtin_exit(int ac, char **av);
int		builtin_unset(t_minishell *msh, int ac, char **av);
int		builtin_env(t_minishell *msh, int ac, char *av[]);

#endif
