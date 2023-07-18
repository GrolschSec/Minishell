/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:56:04 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/18 22:07:39 by rlouvrie         ###   ########.fr       */
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
	while (i < data->pipes - 1)
	{
		if (data->exec[i].no_cmd)
		{
			i++;
			continue ;
		}
		else if (process_creation(data, &data->exec[i]) < 0)
		{
			execution_handling(data, i);
			return ;
		}
		i++;
	}
	data->exec[i].is_last = 1;
	if (!data->exec[i].no_cmd)
		exec_last_child(data, &data->exec[i]);
	end_exec(data);
	return ;
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

	pipe(fd);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid != 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		if (exec->cmd[0] && is_builtin(exec->cmd[0]))
			wait(NULL);
		close(fd[0]);
	}
	else if (pid == 0)
	{
		ps_crea_child_exec(data, exec, fd);
		return (-1);
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

	path = NULL;
	if ((exec->cmd[0][0] == '/' || exec->cmd[0][0] == '.')
			&& !is_builtin(exec->cmd[0]))
		path = ft_strdup(exec->cmd[0]);
	else if (!is_builtin(exec->cmd[0]))
		path = get_cmd_path(exec->cmd[0], data);
	if (exec->fdin < 0 || exec->fdout < 0
		|| (!path && !is_builtin(exec->cmd[0])))
		return (free(path), -1);
	if (exec->fdin > STDIN_FILENO)
		dup2(exec->fdin, STDIN_FILENO);
	if (exec->fdout > STDOUT_FILENO)
		dup2(exec->fdout, STDOUT_FILENO);
	if (is_builtin(exec->cmd[0]))
		select_builtin(data, exec);
	else
		do_execve(data, exec, path);
	return (-1);
}

/*
 * exec_last_child handles the execution of the last command in a pipeline.
 * For the exit built-in command, some cleanup is done without execution.
 * For export, unset, and cd built-ins, 
 * command is executed directly without new process.
 * For other commands, a new process is created for execution.
 * data: Global runtime information.
 * exec: Information about the command to be executed.
 * Returns: 0
 */
int	exec_last_child(t_data *data, t_exec *exec)
{
	if (!exec->cmd[0] && exec->heredoc > 0)
		return (0);
	if (is_builtin(exec->cmd[0]))
		command_exec(data, exec);
	else if (last_child(data, exec) < 0)
	{
		close(data->cpy_in);
		close(data->cpy_out);
		return (exit_ps(data, g_exit), 0);
	}
	return (0);
}

/*
 * last_child creates a new process and executes the command in the child.
 * The parent waits for the child and retrieves the exit status.
 * If the command is not built-in and not a local executable, 
 * an error code is set.
 * data: Global runtime information.
 * exec: Information about the command to be executed.
 * Returns: 0 after setting signal handlers.
 */
int	last_child(t_data *data, t_exec *exec)
{
	int	pid;

	pid = fork();
	signal(SIGQUIT, ft_signal_quit);
	signal(SIGINT, ft_signal_newline2);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		if (command_exec(data, exec) < 0)
		{
			if (exec->cmd[0] && !is_builtin(exec->cmd[0])
				&& exec->cmd[0][0] != '.' && exec->cmd[0][0] != '/')
				dot_slash_cmd_exec_handling(exec->cmd[0], data);
			clear_fd(data);
			return (-1);
		}
	}
	else if (pid)
		last_child_main(pid, exec);
	return (signal(SIGINT, ft_signal_newline), signal(SIGQUIT, SIG_IGN), 0);
}
