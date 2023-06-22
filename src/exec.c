/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:56:04 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/06/22 14:53:01 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	return ; //cleaning and back to prompt
}

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

int	command_exec(t_data *data, t_exec *exec)
{
	char	*path;

	if ((exec->cmd[0][0] == '/' || exec->cmd[0][0] == '.') && !is_builtin(exec->cmd[0]))
		path = ft_strdup(exec->cmd[0]);
	else if (!is_builtin(exec->cmd[0]))
		path = get_cmd_path(exec->cmd[0], data);
	printf("%s", path);
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

char	*get_cmd_path(char *cmd, t_data *data)
{
	int		i;
	char	**path;
	char	*cmd_path;
	char	*tmp;
	
	(void)data;
	path = ft_split(getenv("PATH"), ':');
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (free_tab_exec(path, i), NULL);
		cmd_path = ft_strjoin(tmp, cmd);
		if (!cmd_path)
			return (free_tab_exec(path, i), NULL);
		if (access(cmd_path, X_OK) == 0)
			return (free_tab_exec(path, i), cmd_path);
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (free_tab(path), NULL);
}

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

int	exec_last_child(t_data *data, t_exec *exec)
{
	if (is_builtin(exec->cmd[0]) == EXIT)
	{
		close(data->cpy_in);
		close(data->cpy_in);
		//cleaning
	}
	else if (is_builtin(exec->cmd[0]) == EXPORT 
		|| is_builtin(exec->cmd[0]) == UNSET 
		|| is_builtin(exec->cmd[0]) == CD)
	{
		command_exec(data, exec);
	}
	else if (last_child(data, exec) < 0)
	{
		close(data->cpy_in);
		close(data->cpy_out);
		return (0); /*clening*/
	}
	return (0);
}

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
	return (signal(SIGINT, ft_handler), signal(SIGQUIT, SIG_IGN), 0);
}
