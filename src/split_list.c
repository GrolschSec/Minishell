/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:53:43 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/01 15:13:00 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	split_meta(t_data *data, int *i, int *j)
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

void	if_is_quote(t_data *data, int *i, int *j)
{
	if (*i > 0 && is_space(data->str[*i - 1]) == 0
		&& is_meta(data->str[*i - 1]) == 0)
		add_in_previous_node(data, i, j);
	else
		split_quote(data, i, j, data->str[*i]);
}

void	if_is_else(t_data *data, int *i, int *j)
{
	if (*i > 0 && (data->str[*i - 1] == '\"' || data->str[*i - 1] == '\''))
		add_in_previous_node(data, i, j);
	else
	{
		while (data->str[*i] && (is_meta(data->str[*i]) == 0
				&& is_space(data->str[*i]) == 0
				&& data->str[*i] != '\"' && data->str[*i] != '\''))
			(*i)++;
		add_node(data, *i, *j);
		(*i)--;
	}
}

void	split_in_list(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (data->str[i])
	{
		if (is_meta(data->str[i]) == 1)
			split_meta(data, &i, &j);
		else if (data->str[i] == '\"' || data->str[i] == '\'')
			if_is_quote(data, &i, &j);
		else if (data->str[i] == ' ')
		{
			while (data->str[i] == ' ')
				i++;
			i--;
		}
		else if (is_meta(data->str[i]) == 0 && is_space(data->str[i]) == 0)
			if_is_else(data, &i, &j);
		if (data->str[i])
		{
			i++;
			j = i;
		}
	}
}
