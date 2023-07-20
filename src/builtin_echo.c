/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:53:17 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/20 13:11:47 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	echo_builtin(t_exec *exec)
{
	int	i;
	int	r;

	r = 1;
	i = 1;
	check_options(exec, &i, &r);
	while (exec->cmd[i])
	{
		ft_putstr_fd(exec->cmd[i], STDOUT_FILENO);
		if (exec->cmd[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (r)
		ft_putchar_fd('\n', STDOUT_FILENO);
	g_exit = 0;
}

void	check_options(t_exec *exec, int *i, int *r)
{
	while (exec->cmd[*i])
	{
		if (is_n_options(exec->cmd[*i]))
		{
			*r = 0;
			*i += 1;
		}
		else
			return ;
	}
}

int	is_n_options(char *cmd)
{
	int	i;

	i = 0;
	if (cmd[i] == '-' && ft_strlen(cmd) == 1)
		return (0);
	if (cmd[i] == '-')
		i++;
	else
		return (0);
	while (cmd[i])
	{
		if (cmd[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
