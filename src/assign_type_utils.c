/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_type_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 18:12:25 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/30 17:13:01 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Function: len_is_one
** ---------------------
** Assigns the corresponding type to a token based on its first character
** if its length is one.
**
** Args:
** - data: Pointer to the main data structure.
** - tmp: Pointer to the token list.
**
** Side effects:
** Assigns a type to the token based on its content.
** For '&' content, exits with an error message.
*/
void	len_is_one(t_data *data, t_list *tmp)
{
	if (tmp->content[0] == '|' && tmp->type == 0)
		tmp->type = PIPE;
	if (tmp->content[0] == '<' && tmp->type == 0)
		tmp->type = REDIRECT_INPUT;
	if (tmp->content[0] == '>' && tmp->type == 0)
		tmp->type = REDIRECT_OUTPUT;
	if (tmp->content[0] == '&' && tmp->type == 0)
	{
		tmp->type = EPERLUETTE;
		exit_all(data, 1, "There is a problem in the input");
	}
	if ((tmp->content[0] == '(' || tmp->content[0] == ')') && tmp->type == 0)
		tmp->type = PARENTHESIS;
	if (tmp->content[0] == ';' && tmp->type == 0)
		tmp->type = SEMICOLON;
	if (tmp->content[0] == ' ' && tmp->type == 0)
		tmp->type = BLANCK;
}

/*
** Function: len_is_two
** ---------------------
** If a token's length is two, assigns corresponding type based on its content.
**
** Args:
** - tmp: Pointer to the token list.
**
** Side effects:
** Assigns a type to the token based on its content.
*/
void	len_is_two(t_list *tmp)
{
	if (tmp->content[0] == '<' && tmp->content[1] == '<' && tmp->type == 0)
	{
		tmp->type = HEREDOC;
		tmp->next->type = ENDOFFILE;
	}
	if (tmp->content[0] == '>' && tmp->content[1] == '>' && tmp->type == 0)
	{
		tmp->type = DELIMITER_APPEND;
		tmp->next->type = OUTFILE;
	}
}

/*
** Function: type_dollar
** ----------------------
** Assigns the corresponding type to a token starting with '$'.
**
** Args:
** - tmp: Pointer to the token list.
**
** Side effects:
** Assigns a type to the token based on its content.
*/
void	type_dollar(t_data *data, t_list *tmp)
{
	char	*variable;

	variable = NULL;
	if (ft_strlen(tmp->content) == 2 && tmp->content[1] == '?')
	{
		tmp->type = EXIT_STATUS;
	}
	else
	{
		tmp->type = ENVIRONMENT_VARIABLE;
		variable = ft_getenv(data, &tmp->content[1]);
		free(tmp->content);
		tmp->content = ft_strdup(variable);
		free(variable);
	}
}

/*
** Function: type_option
** ----------------------
** Determines if a token starting with '-' is a number or an option.
**
** Args:
** - tmp: Pointer to the token list.
**
** Side effects:
** Assigns a type to the token based on its content.
*/
void	type_option(t_list *tmp)
{
	size_t	i;

	i = 1;
	while (tmp->content[i] && ft_isdigit(tmp->content[i]) == 1)
		i++;
	if (ft_strlen(tmp->content) == i)
		tmp->type = NUMBER;
	else
		tmp->type = OPTION;
}
