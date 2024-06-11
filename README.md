# Minishell

# Table of Contents
1. [Description](#description)
2. [Installation Instructions](#installation-instructions)
3. [Usage Instructions](#usage-instructions)
4. [Key Features](#key-features)
5. [Contribution Guidelines](#contribution-guidelines)
6. [Acknowledgments](#acknowledgments)
7. [License Information](#license-information)
8. [Contact Information](#contact-information)
9. [Project Development](#project-development)

## Description

Minishell is a simplified command-line interpreter implemented in C. It's a minimalistic shell that replicates some of the functionalities of common shells like Bash or Zsh. 

Key features of Minishell include:

- Displaying a prompt when waiting for a new command.
- Maintaining a working history of commands.
- Searching and launching the right executable (based on the PATH variable or using a relative or an absolute path).
- Handling signals with minimal use of global variables.
- Interpreting single (' ') and double (" ") quotes, with special handling for meta-characters and the $ sign.
- Implementing redirections (<, >, <<, >>) and pipes (|).
- Handling environment variables ($ followed by characters) and the $? variable.
- Handling ctrl-C, ctrl-D, and ctrl-\ signals in a manner similar to bash.
- Implementing several builtins: echo with option -n, cd with only a relative or absolute path, pwd with no options, export with no options, unset with no options, env with no options or arguments, and exit with no options.
- Implementing logical operators (&& and ||) with parentheses for priority.
- Implementing wildcard (*) functionality, which works not only for the current working directory but also for other directories.

While Minishell is not as feature-rich as full-fledged shells, it provides a platform to run text-based applications and perform basic system operations. It serves as an excellent learning tool for those interested in system programming, process synchronization, and the inner workings of a command-line interface.

## Installation Instructions

Before you begin, ensure you have met the following requirements:

1. **Download the project**: You can clone the project from GitHub using the following command in your terminal:
```bash
git clone https://github.com/FirePh0enix/minishell
```

2. **Install a C compiler**: If you don't already have a C compiler installed, you will need one to build and use this library. You can install the [Clang compiler](https://clang.llvm.org).
   
- On a Mac, you should already have Clang installed as part of Xcode Command Line Tools. You can confirm this by running clang --version in your terminal. If it's not installed, you'll be prompted to install it.

- On a Linux machine, use the package manager for your distribution. For example, on Ubuntu:
```bash
sudo apt install clang
```

## Usage Instructions

Follow these steps to use the Minishell project:

1. **Compile the Project:** Navigate to the cloned repository and compile the project using the provided Makefile. Execute the following command in your terminal:
```bash
make
```

2. **Run the Program:**

To start the Minishell, use the following command:
```bash
./minishell
```
This will launch the Minishell command-line interface where you can start executing commands. The specific features and commands supported by Minishell are detailed in the 'Key Features' section.

## Key Features

The development of this project was divided into two main parts: Execution and Parsing.

### Execution

I was responsible for the execution part, handling the running and management of commands and coding some builtins.

#### Overall Idea

* The overall concept was initially challenging for me, as I had never worked with a tree structure before. The idea was to navigate through this tree recursively, extracting necessary information such as which file descriptor I should read/write from, whether I need to create a pipe, the command name, its options, and so on.

* The core of the execution process involves several steps. The first step is to check the redirections: can we open all the necessary file descriptors?

* The next step is to check if the command is a builtin. If it is, we won't fork and will execute the builtin directly.

* The final step is to determine whether I'm dealing with a single command or other tokens such as parentheses, pipe (`|`), or logical operators (`||` and `&&`). The process differs slightly when I'm not dealing with a command node. I will explain this in more detail later.
```C
int	exec_cmd(t_minishell *msh, t_node *node, int parent_in, int parent_out)
{
	if (node->type == TY_CMD)
	{
		if (check_all_redirects(node) == -1)
			return (1);
		if (is_builtin(node))
			return (exec_builtin(msh, node, parent_in, parent_out));
		else
			return (create_child(msh, node, parent_in, parent_out));
	}
	return (handle_if_not_cmd(msh, node, parent_in, parent_out));
}
```

#### Checking Redirections
* Here we are verifying that each infile and outfile can be opened without a problem.
```C
static int	check_all_redirects(t_node *node)
{
	size_t	i;
	int		fd;
	t_red	red;

	i = 0;
	while (i < ft_vector_size(node->cmd.all_reds))
	{
		red = node->cmd.all_reds[i];
		if (red.type == RED_IN)
			fd = open(red.filename, O_RDONLY);
		else if (red.type == RED_OUT && red.append)
			fd = open(red.filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
		else if (red.type == RED_OUT && !red.append)
			fd = open(red.filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd == -1)
		{
			msh_errno(red.filename);
			return (-1);
		}
		close(fd);
		i++;
	}
	return (0);
}
```

#### Checking builtins
* Here, we are simply detecting whether the command needs to be executed by one of our built-in functions, or if we should fork it and then execute it.
```C
int	exec_builtin(t_minishell *msh, t_node *node, int in, int out)
{
	int	ac;

	ac = node->cmd.argc;
	if (ft_strcmp(node->cmd.argv[0], "cd") == 0)
		return (builtin_cd(msh, in, out, node));
	else if (ft_strcmp(node->cmd.argv[0], "pwd") == 0)
		return (builtin_pwd(msh, in, out, node));
	else if (ft_strcmp(node->cmd.argv[0], "echo") == 0)
		return (builtin_echo(ac, node->cmd.argv, out, node));
	else if (ft_strcmp(node->cmd.argv[0], "exit") == 0)
		return (builtin_exit(msh, in, out, node));
	else if (ft_strcmp(node->cmd.argv[0], "unset") == 0)
		return (builtin_unset(msh, ac, node->cmd.argv, node));
	else if (ft_strcmp(node->cmd.argv[0], "env") == 0)
		return (builtin_env(msh, in, out, node));
	else if (ft_strcmp(node->cmd.argv[0], "export") == 0)
		return (builtin_export(msh, in, out, node));
	return (0);
}
```

#### Handling a command

* The concept here is to create a child process for executing a command. We then add the PID (Process ID) of the child to a vector. This allows us to terminate the child process if necessary, for instance, in the case of a Ctrl + C command. In the parent process, we close unnecessary file descriptors and then wait for the child process to complete.
```C
int	create_child(t_minishell *msh, t_node *node, int in, int out)
{
	int		pid;

	if (node->cmd.argc == 0)
		add_env(msh, node);
	pid = fork();
	if (pid == -1)
		return (msh_errno(""), -1);
	if (pid == 0)
		exec_child(msh, node, in, out);
	ft_vector_add(&msh->child_pids, &pid);
	close_fd_parent(in, out);
	if (!node->parent || node->parent->type != TY_PIPE)
		return (wait_for_children(msh));
	return (0);
}
```

* Lets see in depth of exec_child function works:

* First step is to create the part that will be used in execve, if the path is found with access function we will go to the next step which is the setup of dup. Final step is executing the command.

```C
static int	exec_child(t_minishell *msh, t_node *node, int in, int out)
{
	char	*cmd;

	cmd = NULL;
	if (node->cmd.argc > 0)
	{
		cmd = ft_create_path(msh, node->cmd.argv[0]);
		if (!cmd)
		{
			msh_error_cmd(node->cmd.argv[0]);
			free_when_no_cmd(msh, node);
			errno = ENOENT;
			return (exit(code_for_errno()), 0);
		}
		else if (cmd == (void *) 1 || cmd == (void *) 2)
		{
			msh_errno(node->cmd.argv[0]);
			return (free_when_no_cmd(msh, node), exit(code_for_cmd(cmd)), 0);
		}
	}
	add_env(msh, node);
	overall_dup(node, in, out);
	close_fd_child(msh);
	if (cmd)
		exec_cmd_when_cmd_ok(msh, cmd, node);
	free_at_end(msh, node);
}
```

* I'm using `dup2` to specify where a child process should read from and write to. The variables `in` and `out` provide information about the pipe: `in` is used when I need to read from the pipe, and `out` is used when I need to write to it. `infile` and `outfile` serve similar purposes, but they are used for file input and output, respectively.
```C
void	overall_dup(t_node *node, int in, int out)
{
	if (in != -1)
		dup_in(in);
	if (node->cmd.infile)
		dup_infile(node);
	if (out != -1)
		dup_out(out);
	if (node->cmd.outfile)
		dup_outfile(node);
	return ;
}
```

* After the setup of dup2, i can now execute a command. We are making sure that the fd in not a folder then executing with all infos found in the node. If execve fails, you need to free every malloc properly, if execve succed then you don t need to manage it.
```C
int	ft_exec_cmd(t_minishell *msh, char *cmd, char **av, char **envp)
{
	int	fd;

	fd = open(cmd, O_DIRECTORY | O_RDONLY);
	if (fd != -1)
	{
		close(fd);
		errno = EISDIR;
		msh_errno(cmd);
		free_in_exec(msh, cmd, av);
		free(cmd);
		exit(126);
	}
	errno = 0;
	if (execve(cmd, av, envp) == -1)
	{
		msh_errno(cmd);
		free_in_exec(msh, cmd, av);
		exit(code_for_errno());
	}
	free(cmd);
	return (0);
}
```

#### Handling other tokens

* Lets get back in time and see how i manage when the node is not a command

* Thos function will identify the type of token in the node and manage it as follows.
```C
int	handle_if_not_cmd(t_minishell *msh, t_node *node, int in, int out)
{
	if (node->type == TY_PIPE)
		return (handle_pipe(msh, node, in, out));
	else if (node->type == TY_OR)
		return (handle_or(msh, node, in, out));
	else if (node->type == TY_AND)
		return (handle_and(msh, node, in, out));
	else if (node->type == TY_PARENT)
		return (handle_parent(msh, node, in, out));
	return (0);
}
```

To handle pipes, I create a pipe and add the two new file descriptors given by the pipe function, so I can easily free them later. One challenge I faced was not knowing that if we encounter multiple pipe tokens, I will create pipes and these pipes will be "waiting". This means that when I encounter a command, the child process will be responsible for closing all of these pipes. Therefore, it's a good idea to store these file descriptors.

Here's the recursive approach: when the node in the tree is a pipe, we will call the function `exec_cmd` again, this time with the left and right branches. If it encounters a new pipe, the process will be repeated. However, if the node to the left or right is a command, then we will execute it. I'm updating the status after each recursive call, and at the end, we wait as usual.
```C
int	handle_pipe(t_minishell *msh, t_node *node, int in, int out)
{
	int	fd[2];
	int	status;

	if (pipe(fd) == -1)
		return (msh_errno(""), 1);
	ft_vector_add(&msh->open_fds, &fd[0]);
	ft_vector_add(&msh->open_fds, &fd[1]);
	status = exec_cmd(msh, node->pipe.left, in, fd[1]);
	close(fd[1]);
	status = exec_cmd(msh, node->pipe.right, fd[0], out);
	close(fd[0]);
	status = wait_for_children(msh);
	return (status);
}
```

* This function is responsible for handling the case when the node is a `||` token. If this is the case, I will recursively call the `exec_cmd` function with the left and right branches. If the left branch is a command, it will proceed with the command process. After that, I'm checking if the command executes successfully. If it does, there's no need to execute the command in the right branch, as this is how the `||` operator works.
```C
int	handle_or(t_minishell *msh, t_node *node, int in, int out)
{
	int	status;

	status = exec_cmd(msh, node->pipe.left, in, out);
	if (status == 0)
		return (status);
	status = exec_cmd(msh, node->pipe.right, in, out);
	return (status);
}
```

* This function handles the case when the node is an `&&` token. It's quite similar to the previous case, with the difference being that if the first command succeeds, we can execute the next command.
```C
int	handle_and(t_minishell *msh, t_node *node, int in, int out)
{
	int	status;

	status = exec_cmd(msh, node->pipe.left, in, out);
	if (status != 0)
		return (status);
	status = exec_cmd(msh, node->pipe.right, in, out);
	return (status);
}
```

* This function handles parentheses. We have a specific node that contains this case. Thanks to the incredible parsing, I can simply execute it with the node dedicated for this case.
```C
int	handle_parent(t_minishell *msh, t_node *node, int in, int out)
{
	int			status;

	status = exec_cmd(msh, node->pa.node, in, out);
	return (status);
}
```

That covers most of the execution part. I didn't think discussing the closing of file descriptors would be interesting, so I skipped that part. However, feel free to delve deeper if needed. The relevant details can be found in the `close_fd.c` file.

### Parsing

My partner took charge of the parsing part, dealing with the interpretation of command-line input.



## Acknowledgments



## Contribution Guidelines

I welcome contributions from everyone. Here are some guidelines to follow:

1. **Fork the repository**: Start by forking the repository to your own GitHub account.

2. **Clone the repository**: Clone the forked repository to your local machine.
```bash
git clone https://github.com/FirePh0enix/minishell
```

3. **Create a new branch**: Create a new branch for each feature or bug fix you're working on. Do not make changes directly on the master branch
```bash
git checkout -b your-branch-name
```

4. **Make your changes**: Make your changes in the new branch. Ensure your code follows the [norminette](https://github.com/42School/norminette).

5. **Commit your changes**: Commit your changes regularly with clear, descriptive commit messages.
```bash
git commit -m "Your commit message"
```

6. **Push your changes**: Push your changes to your forked repository on GitHub.
```bash
git push origin your-branch-name
```

7. **Create a pull request**: Go to your forked repository on GitHub and create a new pull request against the master branch.
Please note that this project has a code of conduct, and contributors are expected to adhere to it. Any contributions you make are greatly appreciated.

## License Information



## Contact Information

If you have any questions, issues, or if you want to contribute, feel free to reach us out:

- GitHub: [@FirePh0enix](https://github.com/FirePh0enix)
- GitHub: [@Vpekdas](https://github.com/Vpekdas)
- Discord: Captain-Plouf#7811

## Project Development

### Development Process



### Challenges and Solutions



### Tools and Technologies Used

The "Minishell" project was developed using C.

### Lessons Learned



### Future Plans

There are currently no plans to further improve the "Minishell" project. The experience gained from this project has been invaluable and will be applied to future projects.

### Current Status

The project is currently complete and not in active development. However, maintenance and updates will be done as needed.

### Future Plans

Plans for future development include adding more functions, improving performance, and expanding the documentation.

### Known Issues

There are currently no known issues. If you find a bug, please report it in the [issue tracker](https://github.com/FirePh0enix/minishell/issues).

### Contributing

Contributions are always welcome! See the [Contribution Guidelines](#contribution-guidelines) for more information.
