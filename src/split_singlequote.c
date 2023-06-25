/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_singlequote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:10:49 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/25 17:45:18 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*extract_variable(t_data *data, char *str, int *i, int *j)
{
	char	*variable;

	variable = NULL;
	while (str[*i] && str[*i] != '\'')
	{
		(*i)++;
	}
	if (*i == (int)ft_strlen(&str[0]))
		exit_all(data, 1, "There is a double quote missing");
	variable = malloc(sizeof(char) * ((*i - *j) + 1));
	if (variable == NULL)
		exit_all(data, 1, "problem malloc variable split list");
	ft_strlcpy(variable, &str[*j], ((*i - *j) + 1));
	(*i)++;
	return (variable);
}

static void	add_variable_quoted(t_data *data, char *str, int *i, int *j)
{
	t_list	*tmp;
	char	*variable;

	variable = NULL;
	tmp = data->token_list;
	(*i)++;
	if (str[*i])
		*j = *i;
	else
		return ;
	while (tmp->next != NULL)
		tmp = tmp->next;
	variable = extract_variable(data, str, i, j);
	*j = *i;
	(*i)--;
	tmp->content = ft_strjoin(tmp->content, variable);
	tmp->type = ENVIRONMENT_VARIABLE;
	free(variable);
}

/*
** split_single_quotes: Handles the parsing of a single quoted string
** in the user input and adds a new node to the token list accordingly.
**
** Args:
**   data: Pointer to the data structure where the token list is located.
**   str: The user input string.
**   i: Pointer to the current index in the string.
**   j: Pointer to the start index of the substring.
**
** Side effects:
**   If an error occurs, the program exits with an error message.
*/
void	split_single_quotes(t_data *data, char *str, int *i, int *j)
{
	if (str[*i - 1] == '=')
		return (add_variable_quoted(data, str, i, j));
	(*i)++;
	if (str[*i])
		*j = *i;
	else
		return ;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] == '\'')
	{
		add_node_single_quote(data, str, *i, *j);
		if (str[*i])
		{
			(*i)++;
			*j = *i;
			(*i)--;
		}
		else
			return ;
	}
	else
		exit_all(data, 1, "There is a single quote missing");
}
