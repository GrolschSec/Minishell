/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:53:43 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 18:58:55 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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


/*
** extract_variable: Extracts a variable name from the user input string
** enclosed in double quotes.
**
** Args:
**   data: Pointer to the data structure where the token list is located.
**   str: The user input string.
**   i: Pointer to the current index in the string.
**   j: Pointer to the start index of the substring.
**
** Returns:
**   Pointer to the extracted variable name.
**
** Side effects:
**   If an error occurs, the program exits with an error message.
*/
char	*extract_variable(t_data *data, char *str, int *i, int *j)
{
	char	*variable;

	variable = NULL;
	while (str[*i] && str[*i] != '\"')
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

/*
** add_variable_quoted: Adds a new node to the token list containing 
** a variable name extracted from a quoted string in the user input.
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
** split_double_quotes: Handles the parsing of a double quoted string
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
static void	split_double_quotes(t_data *data, char *str, int *i, int *j)
{
	//int	quote;

	//quote = 0;
	if (str[*i - 1] == '=')
	{
		add_variable_quoted(data, str, i, j);/* si variable_value entre doubel quotes - a faire aussi pour single quotes */
		return ;
	}
	(*i)++;
	if (str[*i])
		*j = *i;
	else
		return ;
	while (str[*i] && str[*i] != '\"')
	{
		(*i)++;
	}
	if (str[*i] == '\"')
	{
		add_node_double_quote(data, str, *i, *j);
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
		exit_all(data, 1, "There is a double quote missing");
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
static void	split_single_quotes(t_data *data, char *str, int *i, int *j)
{
	(*i)++;
	if (str[*i])
		*j = *i;
	else
		return ;
	while (str[*i] && str[*i] != '\'')
	{
		(*i)++;
	}
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
