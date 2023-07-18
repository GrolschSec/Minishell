/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 20:57:54 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/18 11:44:51 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	dot_slash_cmd_exec_handling(char *cmd, t_data *data)
{
	int		fd;
	char	*path;

	if (ft_strchr(cmd, '/'))
	{
		fd = open(cmd, O_RDWR);
		if (errno != EISDIR && fd > 0)
			close(fd);
		exec_error(cmd, strerror(errno));
		g_exit = 126;
	}
	else
	{
		path = ft_getenv(data, "PATH");
		if (!path)
			exec_error(cmd, "No such file or directory");
		else
		{
			exec_error(cmd, "command not found");
			free(path);
		}
		g_exit = 127;
	}
}

char	**get_path(t_data *data)
{
	char	*path;
	char	**path_tab;

	path = ft_getenv(data, "PATH");
	if (!path)
		return (NULL);
	path_tab = ft_split(path, ':');
	free(path);
	if (!path_tab)
		return (NULL);
	if (!path_tab[0])
		return (free_tab(path_tab), NULL);
	return (path_tab);
}

void	do_execve(t_data *data, t_exec *exec, char *path)
{
	put_env_in_tab(data);
	execve(path, exec->cmd, data->env_tab);
	dot_slash_cmd_exec_handling(exec->cmd[0], data);
	if (data->env_tab)
		free_tab(data->env_tab);
	if (path)
		free(path);
}
