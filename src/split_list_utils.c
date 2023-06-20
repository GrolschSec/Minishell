/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 15:08:29 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/18 19:46:02 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_metacharacter(char c)
{
	if (c == '|' || c == '&' || c == ';')
		return (1);
	if (c == '(' || c == ')')
		return (1);
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	if (c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

char	*fill_tmp(char *str, int len)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = NULL;
/*	if (len <= 0)
		len = 0;*/
	if (len <= 0 || !str || str == NULL)
		return (NULL);
	tmp = malloc(sizeof(char) * (len + 1));
	if (!tmp || tmp == NULL)
		return (NULL);
	while (i < len)
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

void	add_node(t_data *data, char *str, int i, int j)
{
	char	*tmp;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	tmp = fill_tmp(&str[j], i - j);
	if (tmp == NULL || str == NULL)
		exit_all(data, 1, "There is a problem with malloc when I try to add a node in the token list");
	new = ft_lstnew(tmp);
	free (tmp);
	ret = ft_lstadd_back(&data->token_list, new);
	if (ret == 1)
		exit_all(data, 1, "There is a problem with malloc when I try to add a node in the end of token list");
}

void	add_node_double_quote(t_data *data, char *str, int i, int j)
{
	char	*tmp;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	tmp = fill_tmp(&str[j], i - j);
	if (tmp == NULL || str == NULL)
		exit_all(data, 1, "Problem with malloc when adding a double quoted node to the token list");
	new = ft_lstnew(tmp);
	new->type = DOUBLE_QUOTE;
	if (j > 1)
	{
		if (str[j - 2] == '=')
			new->type = VARIABLE_VALUE;/* ici, changer pour que soit dans meme node que precedent */
	}
	free (tmp);
	ret = ft_lstadd_back(&data->token_list, new);
	if (ret == 1)
		exit_all(data, 1, "There is a problem with malloc when I try to add a quoted node in the end of token list");
}

void	add_node_single_quote(t_data *data, char *str, int i, int j)
{
	char	*tmp;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	tmp = fill_tmp(&str[j], i - j);
	if (tmp == NULL || str == NULL)
		exit_all(data, 1, "Problem with malloc when adding a single quoted node to the token list");
	new = ft_lstnew(tmp);
	new->type = SINGLE_QUOTE;
	if (j > 1)
	{
		if (str[j - 2] == '=')
			new->type = VARIABLE_VALUE;/* ici, changer pour que soit dans meme node que precedent */
	}
	free (tmp);
	ret = ft_lstadd_back(&data->token_list, new);
	if (ret == 1)
		exit_all(data, 1, "There is a problem with malloc when I try to add a quoted node in the end of token list");
}
