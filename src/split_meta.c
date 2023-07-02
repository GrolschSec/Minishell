/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_meta.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 14:39:47 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/02 14:40:43 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_meta(char c)
{
	if (c == '&' || c == ';')
		return (1);
	if (c == '(' || c == ')' )
		return (1);
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

void	split_meta(t_data *data, int *i, int *j)
{
	*j = *i;
	(*i)++;
	if (data->str[*i])
	{
		if ((data->str[*i] == '>' || data->str[*i] == '<')
			&&data->str[*i] == data->str[*i - 1])
		{
			while (data->str[*i] && (data->str[*i] == data->str[*i - 1]))
				(*i)++;
		}
	}
	add_node(data, *i, *j);
	(*i)--;
}
