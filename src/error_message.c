/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 17:47:41 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/15 17:48:01 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	error(t_data *data, char *str)
{
	g_exit = 2;
	data->error = 1;
	printf("minishell: syntax error near unexpected token ");
	printf("%s\n", str);
}
