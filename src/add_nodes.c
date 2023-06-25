/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 19:11:57 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/23 19:16:24 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
void	add_node(t_data *data, char *str, int i, int j)
{
	char	*tmp;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	tmp = fill_tmp(&str[j], i - j);
	if (tmp == NULL || str == NULL)
		exit_all(data, 1, "Problem with malloc in add_node");
	new = ft_lstnew(tmp);
	free (tmp);
	ret = ft_lstadd_back(&data->token_list, new);
	if (ret == 1)
		exit_all(data, 1, "Problem with malloc in add_node in add_back");
}

/*
** add_node_double_quote: Similar to add_node but the new node is marked 
** as containing a double quoted string. It also checks if the character 
** before the start index of the substring is an '=' and adjusts the 
** node type accordingly.
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
void	add_node_double_quote(t_data *data, char *str, int i, int j)
{
	char	*tmp;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	tmp = fill_tmp(&str[j], i - j);
	if (tmp == NULL || str == NULL)
		exit_all(data, 1, "Problem with malloc in add_node_double_quote");
	new = ft_lstnew(tmp);
	new->type = DOUBLE_QUOTE;
	if (j > 1)
	{
		if (str[j - 2] == '=')
			new->type = VARIABLE_VALUE;
	}
	free (tmp);
	ret = ft_lstadd_back(&data->token_list, new);
	if (ret == 1)
		exit_all(data, 1, "Problem with malloc: add_node_dble_quote, addback");
}

/*
** add_node_single_quote: Similar to add_node but the new node is marked 
** as containing a single quoted string. It also checks if the character 
** before the start index of the substring is an '=' and adjusts the 
** node type accordingly.
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
void	add_node_single_quote(t_data *data, char *str, int i, int j)
{
	char	*tmp;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	tmp = fill_tmp(&str[j], i - j);
	if (tmp == NULL || str == NULL)
		exit_all(data, 1, "Problem with malloc in add_node_single_quote");
	new = ft_lstnew(tmp);
	new->type = SINGLE_QUOTE;
	if (j > 1)
	{
		if (str[j - 2] == '=')
			new->type = VARIABLE_VALUE;
	}
	free (tmp);
	ret = ft_lstadd_back(&data->token_list, new);
	if (ret == 1)
		exit_all(data, 1, "Malloc problem: add_node_single_quote, add_back");
}
