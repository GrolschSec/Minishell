/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 17:05:41 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 17:05:45 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_lines(int i, char **str, int fd)
{
	while (str[i] && (str[i][0] != '>'
		&& str[i][0] != '<' && str[i][0] != '|')) /* a retoucher a mon avis */
	{
		ft_putstr_fd(str[i++], fd);
		if (str[i])
			ft_putchar_fd(' ', fd);
	}
}

void	builtin_echo_str(t_data *data, char **cmd)
{
	int		i;
	int		j;
	int		status;

	(void)data;
	i = 1;
	status = 0;
	while (cmd[i] && cmd[i][0] == '-' && cmd[i][1] == 'n')
	{
		j = 1;
		while (cmd[i][j] == 'n')
			j++;
		if (cmd[i][j] == '\0')
			status = 1;
		i++;
	}
	if (status == 1)
		print_lines(i, cmd, STDOUT_FILENO);
	if (status == 0)
	{
		print_lines(i, cmd, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
}
