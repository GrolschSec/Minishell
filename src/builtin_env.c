/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:49:24 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/17 15:46:25 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	env_builtin(t_data *data, t_exec *exec)
{
	char	*error_msg;
	char	*tmp;
	int		error;

	error = 0;
	if (exec->nb_cmd > 1)
	{
		if (access(exec->cmd[1], X_OK) != 0)
		{
			error = 1;
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
