/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_in_prev_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 17:18:24 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/15 17:45:49 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	add_quoted(t_data *data, int *i, int *j)
{
	int		ret;

	ret = 0;
	(*j)++;
	if (data->str[*j] == '\0')
		msg_error_quote(data, "There is a quote missing");
	if (data->str[*i] == '\'')
	{
		(*i)++;
		while (data->str[*i] && data->str[*i] != '\'')
			(*i)++;
		ret = 1;
	}
	else if (data->str[*i] == '\"')
	{
		(*i)++;
		while (data->str[*i] && data->str[*i] != '\"')
			(*i)++;
	}
	if (data->str[*i] == '\0')
		msg_error_quote(data, "There is a quote missing");
	return (ret);
}

int	add_unquoted(t_data *data, int *i)
{
	while (data->str[*i] && (is_space(data->str[*i]) == 0
			&& is_meta(data->str[*i]) == 0
			&& data->str[*i] != '\'' && data->str[*i] != '\"'))
		(*i)++;
	return (1);
}

void	add_in_previous_node(t_data *data, int *i, int *j, char quote)
{
	char	*to_add;
	t_list	*tmp;
	int		flag;

	flag = 0;
	tmp = data->token_list;
	to_add = NULL;
	*j = *i;
	if (data->str[*i] && (data->str[*i] == '\'' || data->str[*i] == '\"'))
		add_quoted(data, i, j);
	else if (data->str[*i])
		flag = add_unquoted(data, i);
	to_add = fill_tmp(&data->str[*j], *i - *j);
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->content = ft_strjoin(tmp->content, to_add);
	if (quote == '\'')
		tmp->type = SINGLE_QUOTE;
	if (quote == '\"')
		tmp->type = DOUBLE_QUOTE;
	free (to_add);
	if (flag == 1)
		(*i)--;
	*j = *i;
}
