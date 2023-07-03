/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:53:17 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/03 13:55:46 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	echo_builtin(t_data *data, t_exec *exec)
{
	int	i;
	int	r;

	r = 1;
	i = 1;
	check_options(exec, &i, &r);
	while (exec->cmd[i])
	{
		printf("%s", exec->cmd[i]);
		if (exec->cmd[i + 1])
			printf(" ");
		i++;
	}
	if (r)
		printf("\n");
	*data->exit_code = 0;
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
