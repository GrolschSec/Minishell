/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:53:43 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/25 17:15:10 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	split_meta_utils(t_data *data, char *str, int *i, int *j)
{
	while (*i + 1 < (int)ft_strlen(str) && str[*i] == str[*j])
		(*i)++;
	if (*i + 1 < (int)ft_strlen(str))
		add_node(data, str, *i, *j);
	else if (*i + 1 == (int)ft_strlen(str) && str[*i] == str[*j])
	{
		add_node(data, str, *i + 1, *j);
		*j = *i + 1;
		return ;
	}
	else if (*i + 1 == (int)ft_strlen(str) && str[*i] != str[*j])
	{
		add_node(data, str, *i, *j);
		*j = *i;
		return ;
	}
}

/*
** split_meta: Splits metacharacters from user input into individual nodes.
**
** Args:
**   data: Pointer to the data structure where the token list is located.
**   str: The user input string.
**   i: Pointer to the current index in the string.
**   j: Pointer to the start index of the substring.
*/
static void	split_meta(t_data *data, char *str, int *i, int *j)
{
	(*i)++;
	if (str[*i] && *i + 1 < (int)ft_strlen(str) && str[*i] == str[*j])
		split_meta_utils(data, str, i, j);
	else if (str[*i] && *i + 1 == (int)ft_strlen(str))
	{
		add_node(data, str, *i + 1, *j);
		*j = (int)ft_strlen(str);
		return ;
	}
	else
		add_node(data, str, *i, *j);
	while (str[*i] && is_space(str[*i]) == 1)
		(*i)++;
	*j = *i;
	(*i)--;
}

/* A NORMER -- fonction qui repartit l'input  en liste chainee */
/*
** split_in_list: Splits the user input into nodes based on space,
** metacharacters, and quotes, and adds them to the token list.
**
** Args:
**   data: Pointer to the data structure where the token list is located.
**   str: The user input string.
*/
void	split_in_list(t_data *data, char *str)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{	
		if (str[i] && j != i && (is_metacharacter(str[i]) == 1
				|| is_space(str[i]) == 1 || str[i] == '\"' || str[i] == '\''))
		{
			add_node(data, str, i, j);
			while (str[i] && is_space(str[i]) == 1)
				i++;
			j = i;
		}
		if (str[i] && j == i && is_space(str[i]) == 1)
		{
			while (str[i] && is_space(str[i]) == 1)
				i++;
			j = i;
		}
		if (str[i] && i == j && is_metacharacter(str[i]) == 1)
			split_meta(data, str, &i, &j);
		if (str[i] && i == j && str[i] == '\"')
			split_double_quotes(data, str, &i, &j);
		if (str[i] && i == j && str[i] == '\'')
			split_single_quotes(data, str, &i, &j);
		if (str[i])
			i++;
	}
	if (j != i && is_space(str[j]) == 0)
		add_node(data, str, i, j);
}
