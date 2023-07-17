/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 15:08:29 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/17 17:22:50 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	if (c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

/*
** fill_tmp: Allocates memory and fills it with a substring from the 
** provided string starting at the start index up to the length.
**
** Args:
**   str: The original string.
**   len: Length of the substring to be filled in the allocated memory.
**
** Returns:
**   A pointer to the newly allocated memory containing the substring 
**   or NULL in case of an error or if the length is non-positive.
*/
char	*fill_tmp(char *str, int len)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = NULL;
	if (len < 0 || !str || str == NULL)
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

/*
** add_node: Creates a new node with a copy of a substring from the 
** provided string and appends it to the end of the linked list in 
** the provided data structure.
**
** Args:
**   data: Pointer to the data structure where the token list is located.
**   str: The original string.
**   i: The end index of the substring.
**   j: The start index of the substring.
**
** Side effects:
**   If an error occurs, the program exits with an error message.
*/
void	add_node(t_data *data, int i, int j, char quotetype)
{
	char	*tmp;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	tmp = fill_tmp(&data->str[j], i - j);
	if (tmp == NULL || data->str == NULL)
	{
		free (tmp);
		exit_all(data, 1, "Problem with malloc in add_node");
	}
	new = ft_lstnew(tmp);
	if (quotetype != '\0' && quotetype == '\'')
		new->type = SINGLE_QUOTE;
	if (quotetype != '\0' && quotetype == '\"')
		new->type = DOUBLE_QUOTE;
	free (tmp);
	ret = ft_lstadd_back(&data->token_list, new);
	if (ret == 1)
		exit_all(data, 1, "Problem with malloc in add_node in add_back");
}

void	add_empty_node(t_data *data)
{
	char	*tmp;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	tmp = malloc(sizeof(char) * 1);
	if (tmp == NULL || data->str == NULL)
		exit_all(data, 1, "Problem with malloc in add_node");
	tmp[0] = '\0';
	new = ft_lstnew(tmp);
	free (tmp);
	ret = ft_lstadd_back(&data->token_list, new);
	if (ret == 1)
		exit_all(data, 1, "Problem with malloc in add_node in add_back");
}
