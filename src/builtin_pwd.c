/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 14:45:43 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/19 19:14:00 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pwd_builtin(t_exec *exec)
{
	char	*path;

	if (exec->cmd[1] && !parse_pwd(exec->cmd[1]))
	{
		invalid_option_pwd(exec->cmd[1]);
		g_exit = 2;
		return ;
	}
	path = getcwd(NULL, 0);
	if (!path)
	{
		error_cannot_getcwd();
		return ;
	}
	printf("%s\n", path);
	free(path);
	g_exit = 0;
}

int	parse_pwd(char *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return (1);
	if (cmd[i] == '-')
	{
		i++;
		while (cmd[i])
		{
			if (cmd[i] != '-')
				return (0);
			i++;
		}
	}
	return (1);
}

void	invalid_option_pwd(char *cmd)
{
	write(2, "minishell: pwd: ", 16);
	write(2, &cmd[0], 1);
	write(2, &cmd[1], 1);
	write(2, ": invalid option\n", 17);
	write(2, "pwd: usage: pwd\n", 16);
}

void	error_cannot_getcwd(void)
{
	ft_putstr_fd("pwd: error retrieving current directory: ", 2);
	ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
}
