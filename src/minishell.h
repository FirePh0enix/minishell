/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:24:39 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/29 11:38:34 by ledelbec         ###   ########.fr       */
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
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <limits.h>


// # define NOHOME " %d %s $ "
// # define NOHOME_ERR " %d %s $ "

// # define HOME " %d ~%s $ "
// # define HOME_ERR " %d ~%s $ "

# ifndef TEST
#  define TEST 0
# endif

typedef struct s_minishell
{
	int		exit_code;
	char	**env;
	char	**history;

	bool	end;

	t_node	*node_to_free;

	int		*open_fds;
	pid_t	*child_pids;

	bool	no_env;
	bool	no_color;
	char	*init_path;
}	t_minishell;

inline char	*fnohome(t_minishell *msh)
{
	if (msh->no_color)
		return (" %d %s $ ");
	return (" \001\e[1;34m\002%d \001\e[1;32m\002%s "
		"\001\e[0;34m\002$\001\e[0m\002 ");
}

inline char	*fnohome_err(t_minishell *msh)
{
	if (msh->no_color)
		return (" %d %s $ ");
	return (" \001\e[1;31m\002%d \001\e[1;32m\002%s "
		"\001\e[0;34m\002$\001\e[0m\002 ");
}

inline char	*fhome(t_minishell *msh)
{
	if (msh->no_color)
		return (" %d ~%s $ ");
	return (" \001\e[1;34m\002%d \001\e[1;32m\002~%s "
		"\001\e[0;34m\002$\001\e[0m\002 ");
}

inline char	*fhome_err(t_minishell *msh)
{
	if (msh->no_color)
		return (" %d ~%s $ ");
	return (" \001\e[1;31m\002%d \001\e[1;32m\002~%s "
		"\001\e[0;34m\002$\001\e[0m\002 ");
}

# define EXIT_ERRNUM "msh: exit: `%s' is not a numeric argument\n"

# define CWDERA "%s: error retrieving current directory: getcwd: cannot acccess"
# define CWDERB " parent directories: %s\n"

extern int	g_signum;

void	prompt(t_minishell *minishell);
void	init_signals(t_minishell *minishell);

bool	isemptycmd(char *s);

void	set_ctrlc_heredoc(void);
void	set_ctrlc_default(void);
void	set_sigquit_default(void);
void	set_sigquit_dump(void);

void	write_prefix(t_minishell *msh, char buf[]);

void	add_our_history(t_minishell *msh, char *line);
void	load_history(t_minishell *msh);
void	free_history(t_minishell *msh);

typedef struct s_file
{
	char	*file;
	size_t	start;
}	t_file;

t_tok	*wildcard(char *suffix);
t_file	*filter_files(t_file *files, char *filter);

void	copy_env(t_minishell *minishell, char *envp[]);
char	*getourenv(t_minishell *minishell, char *name);
void	setourenv(t_minishell *msh, char *name, char *value);
void	setourenv2(t_minishell *msh, char *name, char *value);

void	msh_error(char *msg);
void	msh_builtin_error(char *builtin, char *msg);
void	msh_error_cmd(char *cmd);
void	msh_errno(char *cmd);
void	msh_errno2(char *cmd, char *msg);

bool	is_valid_var_name(char *s);

int		builtin_cd(t_minishell *msh, int parent_in, int out, t_node *node);
int		builtin_pwd(t_minishell *msh, int parent_in, int out, t_node *node);
int		builtin_echo(int ac, char **av, int parent_out, t_node *node);
int		builtin_exit(t_minishell *msh, int in, int out, t_node *node);
int		builtin_unset(t_minishell *msh, int ac, char **av, t_node *node);
int		builtin_env(t_minishell *msh, int parent_in, int out, t_node *node);
int		builtin_export(t_minishell *msh, int in, int out, t_node *node);
void	goto_end(char *s, size_t *i);

int		open_outfile(t_node *node);
int		set_exit_code(int *exit_code, int exit_code2);
void	sort_str_tab(char *tab[], int size);
bool	is_valid_var_name(char *s);
int		error_chdir(char *path);
void	free_cd(char *fs, char *cdpath);

#endif
