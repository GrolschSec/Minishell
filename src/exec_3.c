/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 20:57:54 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/10 21:50:01 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	dot_slash_cmd_exec_handling(char *cmd)
{
	int	fd;

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
		exec_error(cmd, "command not found");
		g_exit = 127;
	}
}
