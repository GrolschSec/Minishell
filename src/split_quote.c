/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:04:07 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/14 19:06:03 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	msg_error_quote(t_data *data, char *str)
{
	g_exit = 2;
	data->error = 1;
	printf("%s\n", str);
}

void	split_quote(t_data *data, int *i, int *j, char quotetype)
{
	if (ft_strlen(&data->input[*i]) > 1)
		(*i)++;
	else
		msg_error_quote(data, "There is a quote missing");
	if (data->str[*i] == quotetype)
	{
		add_empty_node(data);
		return ;
	}
	*j = *i;
	if (data->str[*i] == '\0')
		msg_error_quote(data, "There is a quote missing");
	while (data->str[*i] && data->str[*i] != quotetype)
	{
		(*i)++;
		if (data->str[*i] == '\0')
			msg_error_quote(data, "There is a quote missing");
	}
	add_node(data, *i, *j, quotetype);
	*j = *i + 1;
}

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

void	split_dollar_quote(t_data *data, int *i, int *j, char quotetype)
{
	t_list	*tmp;

	tmp = data->token_list;
	(*i)++;
	(*j) = *i;
	while (data->str[*j])
	{
		if (data->str[*j] == quotetype)
			break ;
		(*j)++;
	}
	if (data->str[*j] == '\0')
		msg_error_quote(data, "There is a quote missing");
	while (tmp->next != NULL)
		tmp = tmp->next;
	if (tmp->content[0] == '$')
	{
		free(tmp->content);
		tmp->content = ft_substr(data->str, *i, *j - *i);
	}
	*i = *j;
}

void	if_is_quote(t_data *data, int *i, int *j)
{
	if (*i > 0 && is_space(data->str[*i - 1]) == 0
		&& is_meta(data->str[*i - 1]) == 0
		&& data->str[*i - 1] != '$')
		add_in_previous_node(data, i, j, data->str[*i]);
	else if (*i > 0 && data->str[*i - 1] == '$')
		split_dollar_quote(data, i, j, data->str[*i]);
	else
		split_quote(data, i, j, data->str[*i]);
}
