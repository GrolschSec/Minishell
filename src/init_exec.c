/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 17:12:56 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/18 15:17:52 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_exec(t_exec *current, int x)
{
	current[x].cmd = NULL;
	current[x].infile = NULL;
	current[x].outfile = NULL;
	current[x].eof = NULL;
	current[x].fdin = 0;
	current[x].fdout = 1;
	current[x].redirect_input = 0;
	current[x].redirect_output = 0;
	current[x].heredoc = 0;
	current[x].is_last = 0;
	current[x].nb_cmd = 0;
	current[x].is_eof = 0;
	current[x].fail_fd = 0;
}

void	malloc_init_exec(t_data *data)
{
	int	x;

	x = 0;
	data->exec = malloc(sizeof(t_exec) * data->pipes);
	if (data->exec == NULL)
		exit_all(data, 1, "malloc probleme pour structure");
	while (x < data->pipes)
	{
		init_exec(data->exec, x);
		x++;
	}
}
