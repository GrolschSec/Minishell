/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:56:04 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/06/23 13:38:58 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
 * The execution function executes a pipeline of commands.
 * Duplication of standard I/O is done for restoration.
 * It creates processes for all commands except the last one.
 * Then it executes the last command separately.
 * data: Global runtime information.
 */
void	execution(t_data *data)
{
	int	i;

	i = 0;
	data->cpy_in = dup(0);
	data->cpy_out = dup(1);
	while (i < data->pipes - 1) // problem ici pour le calcul de pipes -1 pour last child
	{
		if (process_creation(data, &data->exec[i]) < 0)
		{
			dup2(data->cpy_out, STDOUT_FILENO);
			if (!is_builtin(data->exec[i].cmd[0]) && data->exec[i].cmd[0][0] != '.')
				//printerror command not found and return
			close(data->cpy_in);
			close(data->cpy_out);
			return ; //cleaning
		}
		i++;
	}
	exec_last_child(data, &data->exec[i]);
	end_exec(data);
	return ; //cleaning and back to prompt
}

/*
 * process_creation creates a process to run a command in a pipeline,
 * excluding the last one. A pipe is created for communication.
 * A new process is forked, where output is redirected to the pipe.
 * The parent process redirects input from the pipe and waits for built-ins.
 * data: Global runtime information.
 * exec: Information about the command to be executed.
 * Returns: 0 on success, -1 if fork failed.
 */
int	process_creation(t_data *data, t_exec *exec)
{
	int	fd[2];
	int	pid;

	pipe(fd); // gerer l'erreur de pipe
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid != 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		if (is_builtin(exec->cmd[0]))
			wait(NULL);
		close(fd[0]);
	}
	else if (pid == 0)
	{
		signal(SIGQUIT, ft_handler);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		command_exec(data, exec);
		close(fd[1]);
	}
	return (0);
}

/*
 * command_exec executes a command.
 * If the command is a built-in, it's handled separately.
 * If it's an external command, command path is obtained and 
 * execve is called to replace the current process image.
 * File descriptors are duplicated before the command execution.
 * data: Global runtime information.
 * exec: Information about the command to be executed.
 * Returns: -1 if error occurred during command preparation, -1 if 
 * command path could not be obtained or if it failed to open FD.
 */
int	command_exec(t_data *data, t_exec *exec)
{
	char	*path;

	if ((exec->cmd[0][0] == '/' || exec->cmd[0][0] == '.') && !is_builtin(exec->cmd[0]))
		path = ft_strdup(exec->cmd[0]);
	else if (!is_builtin(exec->cmd[0]))
		path = get_cmd_path(exec->cmd[0], data);
	if (exec->fdin < 0 || exec->fdout < 0 || (!path && !is_builtin(exec->cmd[0])))
		return (free(path), -1);
	if (exec->fdin > STDIN_FILENO)
		dup2(exec->fdin, STDIN_FILENO);
	if (exec->fdout > STDOUT_FILENO)
		dup2(exec->fdout, STDOUT_FILENO);
	if (is_builtin(exec->cmd[0]))
	{
		printf("is_builtin\n");
		//select_builtin(data, exec);
	}
	else
	{
		execve(path, exec->cmd, data->env_tab); // Carefull env tab they could be null.
		//get_error_exec(exec); // the problem with the tab is that you need to remake it each time you run a command
		// if not you could have a problem with a command like `export BLA=5 | echo $BLA`
	}
	return (free(path), -1); // i have to remake the errcode for those functions clearer.
}

/*
 * is_builtin checks if a command is a built-in command.
 * cmd: Command name.
 * Returns: An integer representing the built-in command if it's 
 * a built-in command, or 0 otherwise.
 */
int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", ft_strlen(cmd)) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", ft_strlen(cmd)) == 0)
		return (2);
	else if (ft_strncmp(cmd, "pwd", ft_strlen(cmd)) == 0)
		return (3);
	else if (ft_strncmp(cmd, "export", ft_strlen(cmd)) == 0)
		return (4);
	else if (ft_strncmp(cmd, "unset", ft_strlen(cmd)) == 0)
		return (5);
	else if (ft_strncmp(cmd, "env", ft_strlen(cmd)) == 0)
		return (6);
	else if (ft_strncmp(cmd, "exit", ft_strlen(cmd)) == 0)
		return (7);
	return (0);
}

/*
 * get_cmd_path gets the path to a command from the PATH env variable.
 * The command is checked in each directory in the PATH until a directory
 * is found where the command is executable.
 * cmd: Command name.
 * data: Global runtime information.
 * Returns: A string containing the path to the command, or NULL if the 
 * command was not found in the PATH or if an error occurred.
 */
char	*get_cmd_path(char *cmd, t_data *data)
{
	int		i;
	char	*path;
	char	*tmp;
	
	i = 0;
	while (data->path.tab[i])
	{
		tmp = ft_strjoin2(data->path.tab[i], "/");
		if (!tmp)
			return (NULL);
		path = ft_strjoin2(tmp, cmd);
		free(tmp);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		path = NULL;
		i++;
	}
	return (NULL);
}

/*
 * free_tab_exec frees a null-terminated array of strings, 
 * starting from a specific index.
 * tab: Array of strings.
 * i: Index to start freeing from.
 */
void	free_tab_exec(char **tab, int i)
{
	if (!tab)
		return ;
	if (!tab[i])
		i++;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

/*
 * exec_last_child handles the execution of the last command in a pipeline.
 * For the exit built-in command, some cleanup is done without execution.
 * For export, unset, and cd built-ins, command is executed directly without new process.
 * For other commands, a new process is created for execution.
 * data: Global runtime information.
 * exec: Information about the command to be executed.
 * Returns: 0
 */
int	exec_last_child(t_data *data, t_exec *exec)
{
	if (is_builtin(exec->cmd[0]) == EXIT)
	{
		close(data->cpy_in);
		close(data->cpy_in);
		//cleaning
	}
	else if (last_child(data, exec) < 0)
	{
		close(data->cpy_in);
		close(data->cpy_out);
		return (0); /*cleaning*/
	}
	return (0);
}

char	*ft_strjoin2(char *s1, char const *s2)
{
	char	*dst;
	size_t	len_s1;
	size_t	len_s2;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	dst = malloc((sizeof(char) * len_s1) + (sizeof(char) * len_s2) + 1);
	if (dst == 0 || !dst)
		return (NULL);
	ft_strlcpy(dst, s1, (len_s1 + 1));
	ft_strlcat(dst, s2, (len_s1 + len_s2 + 1));
	return (dst);
}

/*
 * last_child creates a new process and executes the command in the child.
 * The parent waits for the child and retrieves the exit status.
 * If the command is not built-in and not a local executable, an error code is set.
 * data: Global runtime information.
 * exec: Information about the command to be executed.
 * Returns: 0 after setting signal handlers.
 */
int	last_child(t_data *data, t_exec *exec)
{
	int	pid;
	int	status;

	pid = fork();
	// signal(SIGINT, ??);
	// signal(SIGUIT, ??);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		if (command_exec(data, exec) < 0)
		{
			if (exec->cmd[0] && !is_builtin(exec->cmd[0]) && exec->cmd[0][0] != '.')
				return (g_exit = 127, /*error_handling*/0);
			return (-1);
		}
	}
	else if (pid)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit = WEXITSTATUS(status);
	}
	return (0);
}

void	end_exec(t_data *data)
{
	dup2(data->cpy_out, STDOUT_FILENO);
	dup2(data->cpy_in, STDIN_FILENO);
	close(data->cpy_in);
	close(data->cpy_out);
	clear_cmd(data);
}
