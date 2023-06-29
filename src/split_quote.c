/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:04:07 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/29 17:00:41 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    split_quote(t_data *data, int *i, int *j, char quotetype)
{
    (*i)++;
    *j = *i;
    if (data->str[*i] == '\0')
        exit_all(data, 1, "There is a quote missing");
    while (data->str[*i] && data->str[*i] != quotetype)
    {
        (*i)++;
        if (data->str[*i] == '\0')
            exit_all(data, 1, "There is a quote missing");
    }
    add_node(data, *i, *j);
    *j = *i + 1;
}

void    add_in_previous_node(t_data *data, int *i, int *j)
{
    char    *to_add;
    t_list     *tmp;
    int        flag;

    flag = 0;
    tmp = data->token_list;
    to_add = NULL;
    *j = *i;
    if (data->str[*i] && (data->str[*i] == '\'' || data->str[*i] == '\"'))
    {
        (*j)++;
        if (data->str[*j] == '\0')
            exit_all(data, 1, "There is a quote missing");
        if (data->str[*i] == '\'')
        {
            (*i)++;
            while (data->str[*i] && data->str[*i] != '\'')
                (*i)++;
        }  
        else if (data->str[*i] == '\"')
        {
            (*i)++;
            while (data->str[*i] && data->str[*i] != '\"')
                (*i)++;
        }
        if (data->str[*i] == '\0')
            exit_all(data, 1, "There is a quote missing");
    }
    else if (data->str[*i])
    {
        while (data->str[*i] && (is_space(data->str[*i]) == 0
            && is_metacharacter(data->str[*i]) == 0
            && data->str[*i] != '\'' && data->str[*i] != '\"'))
            (*i)++;
        flag = 1;
    }
    to_add = fill_tmp(&data->str[*j], *i - *j);
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->content = ft_strjoin(tmp->content, to_add);
    if (flag == 1 )
        (*i)--;
    *j = *i;
}