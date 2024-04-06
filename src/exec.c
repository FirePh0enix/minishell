/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:37:57 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/06 14:51:58 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

# define debug(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
# define error(fmt, ...) printf("\033[1;31m[%s:%d] " fmt "\033[0m", __FILE__, __LINE__, ##__VA_ARGS__)

bool	is_builtin(t_node *node)
{
	if (node->cmd.argc == 0)
		return (false);
	if (strcmp(node->cmd.argv[0], "cd") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "pwd") == 0)
		return  (true);
	else if (strcmp(node->cmd.argv[0], "echo") == 0)
		return  (true);
	else if (strcmp(node->cmd.argv[0], "exit") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "unset") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "env") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "export") == 0)
		return (true);
	return (false);
}

int	exec_builtin(t_minishell *msh, t_node *node, int parent_out)
{
	if (strcmp(node->cmd.argv[0], "cd") == 0)
		return (builtin_cd(msh, node->cmd.argc, node->cmd.argv, parent_out, node));
	else if (strcmp(node->cmd.argv[0], "pwd") == 0)
		return (builtin_pwd(node->cmd.argc, node->cmd.argv, parent_out, node));
	else if (strcmp(node->cmd.argv[0], "echo") == 0)
		return (builtin_echo(node->cmd.argc, node->cmd.argv, parent_out, node));
	else if (strcmp(node->cmd.argv[0], "exit") == 0)
		return (builtin_exit(node->cmd.argc, node->cmd.argv, node));
	else if (strcmp(node->cmd.argv[0], "unset") == 0)
		return (builtin_unset(msh, node->cmd.argc, node->cmd.argv, node));
	else if (strcmp(node->cmd.argv[0], "env") == 0)
		return (builtin_env(msh, parent_out, node));
	else if (strcmp(node->cmd.argv[0], "export") == 0)
		return (builtin_export(msh, node->cmd.argc, node->cmd.argv, node));
	return (0);
}

int    exec_cmd(t_minishell *msh, t_node *node, int parent_in, int parent_out)
{
	int		pid;
	int		file;
	char	*cmd;
	int		status;
	int		fd[2];
	static int i = 0;
	static int pipe_nb = 0;


	status = 0;
	cmd = NULL;
	if (node->type == TY_CMD)
	{
		i++;
		if (is_builtin(node))
			return (exec_builtin(msh, node, parent_out));
		if (node->cmd.argc > 0)
		{
			cmd = ft_create_path(msh, node->cmd.argv[0]);
			if (!cmd)
				return (msh_error_cmd(node->cmd.argv[0]), -1);
		}
		pid = fork();
		if (pid == -1)
			return (-1);
		if (pid == 0)
		{
			if (node->cmd.infile && node->cmd.argc > 0)
			{
				file = open(node->cmd.infile, O_RDONLY);
				if (file == -1)
					return (printf("ERROR OPEN INFILE\n"), 1);
				if (dup2(file, STDIN_FILENO) == -1)
					return (printf("ERROR DUP2 TMP\n"), 1);
				// debug("(CHILD %d)CLOSING INFILE %d", i, file);
				close(file);
			}
			else if (parent_in != -1)
			{
				if (dup2(parent_in, STDIN_FILENO) == -1)
					return (printf("ERROR DUP2 PARENT_IN\n"), 1);
				// debug("(CHILD %d)CLOSING PARENT_IN %d", i, parent_in);
				close(parent_in);
			}
			if (node->cmd.outfile)
			{
				int flags = O_WRONLY | O_CREAT;
				if (node->cmd.append)
					flags |= O_APPEND;
				else
					flags |= O_TRUNC;
				file = open(node->cmd.outfile, flags, 0666);
				if (file == -1)
					return (error("ERROR OPEN OUTFILE\n"), 1);
				if (dup2(file, STDOUT_FILENO) == -1)
					return (error("ERROR OUTFILE\n"), 1);
				// debug("(CHILD %d)CLOSING OUTFILE %d", i, file);
				close(file);
			}
			else if (parent_out != -1)
			{
				if (dup2(parent_out, STDOUT_FILENO) == -1)
					return (printf("ERROR DUP2 PARENT_OUT\n"), 1);
				// debug("(CHILD %d)CLOSING PARENT_OUT %d", i ,parent_out);
				close(parent_out);
			}
			if (pipe_nb > 0)
				for (int i = 0; i < pipe_nb * 2; i++)
					close(3 + i);
			if (cmd)
				ft_exec_cmd(cmd, node->cmd.argv, msh->env);
			else
				exit(EXIT_SUCCESS);
		}
		else
		{
			while (wait(&status) > 0)
			{
				if (g_signum != -1)
					kill(pid, g_signum);
			}
		}
	}
	else if (node->type == TY_PIPE)
	{
		if (pipe(fd) == -1)
			return (printf("ERROR PIPE\n"), 1);
		pipe_nb++;
		exec_cmd(msh, node->pipe.left, parent_in, fd[1]);
		if (fd[1] != -1)
		{
			// debug("(PARENT)CLOSING fd[1] %d", fd[1]);
			close(fd[1]);
		}
		exec_cmd(msh, node->pipe.right, fd[0], parent_out);
		if (fd[0] != -1)
		{
			// debug("(PARENT)CLOSING fd[0] %d", fd[0]);
			close(fd[0]);
		}
	}
	else if (node->type == TY_OR)
	{
		status = exec_cmd(msh, node->pipe.left, parent_in, parent_out);
		if (status == 0)
			return (status);
		status = exec_cmd(msh, node->pipe.right, parent_in, parent_out);
	}
	else if (node->type == TY_AND)
	{
		status = exec_cmd(msh, node->pipe.left, parent_in, parent_out);
		if (status != 0)
			return (status);
		status = exec_cmd(msh, node->pipe.right, parent_in, parent_out);
	}
	return (status);
}
