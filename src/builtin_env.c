/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:49:24 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/18 19:33:36 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	env_builtin(t_data *data, t_exec *exec)
{
	int		error;

	error = 0;
	if (exec->nb_cmd > 1)
		env_error(exec, &error);
	if (!error)
		print_env_list(data);
}

void	print_env_list(t_data *data)
{
	t_list	*tmp;

	tmp = data->env;
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
}

int	valid_option_env(t_exec *exec)
{
	if (!ft_strncmp(exec->cmd[1], "--", 2) && ft_strlen(exec->cmd[1]) == 2)
	{
		g_exit = 0;
		return (1);
	}
	return (0);
}

int	invalid_option_env(t_exec *exec)
{
	if (exec->cmd[1][0] == '-' && ft_strlen(exec->cmd[1]) == 1)
	{
		g_exit = 0;
		return (1);
	}
	else if (exec->cmd[1][0] == '-' && ft_isprint(exec->cmd[1][1]))
	{
		ft_putstr_fd("env: invalid option -- '", STDERR_FILENO);
		ft_putchar_fd(exec->cmd[1][1], STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		g_exit = 125;
		return (1);
	}
	return (0);
}

void	env_error(t_exec *exec, int	*error)
{
	char	*error_msg;
	char	*tmp;

	if (valid_option_env(exec))
		*error = 0;
	else if (invalid_option_env(exec))
		*error = 1;
	else if (access(exec->cmd[1], X_OK) != 0)
	{
		*error = 1;
		tmp = ft_strjoin2("env: ‘", exec->cmd[1]);
		if (tmp)
		{
			error_msg = ft_strjoin(tmp, "’");
			if (error_msg)
			{
				perror(error_msg);
				free(error_msg);
			}
		}
		if (errno == EACCES)
			g_exit = 126;
		else if (errno == ENOENT)
			g_exit = 127;
	}
}
