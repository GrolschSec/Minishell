/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:53:17 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/06/30 20:03:36 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	echo_builtin(t_exec *exec)
{
	int	i;
	int	r;

	r = 1;
	i = 1;
	while (exec->cmd[i])
	{
		if (i == 1
			&& ft_strncmp(exec->cmd[i], "-n", ft_strlen(exec->cmd[i])) == 0)
			r = 0;
		else
			printf("%s", exec->cmd[i]);
		i++;
	}
	if (r)
		printf("\n");
}
