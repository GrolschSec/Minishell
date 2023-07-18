/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 22:05:18 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/18 22:06:44 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ps_crea_child_exec(t_data *data, t_exec *exec, int *fd)
{
	signal(SIGQUIT, ft_signal_quit);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	command_exec(data, exec);
	clear_fd(data);
	close(fd[1]);
}
