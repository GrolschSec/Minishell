/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:38:32 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/18 19:38:59 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	input_is_only_space(t_data *data, char *input)
{
	int	i;

	i = 0;
	while (input[i] && is_space(input[i]) == 1)
	{
		i++;
	}
	if ((int)ft_strlen(input) == i
		|| (input[i] == ':' && (int)ft_strlen(input) == 1))
	{
		g_exit = 0;
		data->error = 1;
	}
	if (input[i] == '!' && (int)ft_strlen(input) == 1)
	{
		g_exit = 1;
		data->error = 1;
	}
}

void	check_if_nothing(t_data *data)
{
	int	x;

	x = 0;
	while (x < data->pipes)
	{
		if (data->pipes > 1
			&& data->exec[x].nb_cmd == 0
			&& data->exec[x].redirect_input == 0
			&& data->exec[x].redirect_output == 0
			&& data->exec[x].heredoc == 0)
		{
			data->error = 2;
			g_exit = 2;
			return ;
		}
		x++;
	}
}
