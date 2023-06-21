/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:56:04 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/06/21 15:53:45 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execution(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->pipes)
	{
		if (process_creation(data, data->exec[i]) < 0)
		{
			return ; // ERROR HANDLING
		}
		i++;
	}
	// Last child process run 
}

int	process_creation(t_data *data, t_exec exec)
{
	int	fd[2];
	int	pid;

	(void)data;
	(void)exec;
	pipe(fd);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid != 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		// if builtin wait the child process to end to avoid state change making mistakes in others
		close(fd[0]);
	}
	else if (pid == 0)
	{
		signal(SIGQUIT, ft_handler);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		// exec_cmd
		close(fd[1]);
	}
	return (0);
}

int	command_exec(t_data *data, t_exec exec)
{
	char	*path;

	if ((exec.cmd[0][0] == '/' || exec.cmd[0][0] == '.') && !is_builtin(exec.cmd[0]))
		path = ft_strdup(exec.cmd[0]);
	else if (!is_builtin(exec.cmd[0]))
		get_cmd_path(exec.cmd[0]);
	if (exec.fdin < 0 || exec.fdout < 0 || (!path && !is_builtin(exec.cmd[0])))
		return (free(path), -1);
	if (exec.fdin > STDIN_FILENO)
		dup2(exec.fdin, STDIN_FILENO);
	if (exec.fdout > STDOUT_FILENO)
		dup2(exec.fdout, STDOUT_FILENO);
	if (is_builtin(exec.cmd[0]))
		select_builtin(data, exec);
	else
	{
		execve(path, &exec.cmd, data->env_tab); // Carefull env tab they could be null.
		get_error_exec(exec); // the problem with the tab is that you need to remake it each time you run a command
		// if not you could have a problem with a command like `export BLA=5 | echo $BLA`
		
	}
	return (free(path), -1); // i have to remake the errcode for those functions clearer.
}
