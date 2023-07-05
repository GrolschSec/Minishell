/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 18:11:38 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/05 15:51:37 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	if (!data->path.tab[0])
		return (NULL);
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

void	select_builtin(t_data *data, t_exec *exec)
{
	if (is_builtin(exec->cmd[0]) == EXIT)
		exit_builtin(data, exec);
	else if (is_builtin(exec->cmd[0]) == CD)
		cd_builtin(data, exec);
	else if (is_builtin(exec->cmd[0]) == ECHO)
		echo_builtin(exec);
	else if (is_builtin(exec->cmd[0]) == PWD)
		pwd_builtin();
	else if (is_builtin(exec->cmd[0]) == EXPORT)
		export_builtin(data, exec);
	else if (is_builtin(exec->cmd[0]) == UNSET)
		unset_builtin(data, exec);
	else if (is_builtin(exec->cmd[0]) == ENV)
		env_builtin(data, exec);
}

void	execution_handling(t_data *data, int i)
{
	dup2(data->cpy_out, STDOUT_FILENO);
	if (!is_builtin(data->exec[i].cmd[0])
		&& data->exec[i].cmd[0][0] != '.'
		&& data->exec[i].cmd[0][0] != '/')
	{
		exec_error(data->exec[i].cmd[0], "command not found");
		g_exit = 127;
	}
	close(data->cpy_in);
	close(data->cpy_out);
	exit_ps(data, g_exit);
}
