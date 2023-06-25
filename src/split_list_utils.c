/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 15:08:29 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/23 19:17:14 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** is_metacharacter: Checks if a character is a metacharacter.
** Metacharacters include '|', '&', ';', '(', ')', '<', '>'.
**
** Args:
**   c: Character to be checked.
**
** Returns:
**   1 if the character is a metacharacter, 0 otherwise.
*/
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

/*
** is_space: Checks if a character is a space character.
** Space characters include ' ', '\t', '\n', '\v', '\f', '\r'.
**
** Args:
**   c: Character to be checked.
**
** Returns:
**   1 if the character is a space character, 0 otherwise.
*/
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
