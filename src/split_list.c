/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:53:43 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/17 17:16:35 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	if_is_else(t_data *data, int *i, int *j)
{
	if (*i > 0 && (data->str[*i - 1] == '\"' || data->str[*i - 1] == '\''))
		add_in_previous_node(data, i, j, '\0');
	else
	{
		while (data->str[*i] && (is_meta(data->str[*i]) == 0
				&& is_space(data->str[*i]) == 0
				&& data->str[*i] != '\"' && data->str[*i] != '\''))
			(*i)++;
		add_node(data, *i, *j, '\0');
		(*i)--;
	}
}

void	do_split(t_data *data, int *i, int *j)
{
	if (is_meta(data->str[*i]) == 1)
		split_meta(data, i, j);
	else if (data->str[*i] == '\"' || data->str[*i] == '\'')
		if_is_quote(data, i, j);
	else if (data->str[*i] == ' ')
	{
		while (data->str[*i] == ' ')
			(*i)++;
		(*i)--;
	}
	else if (is_meta(data->str[*i]) == 0
		&& is_space(data->str[*i]) == 0)
		if_is_else(data, i, j);
}

void	split_in_list(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (data->str[i])
	{
		do_split(data, &i, &j);
		if (data->str[i])
		{
			i++;
			j = i;
		}
	}
}
