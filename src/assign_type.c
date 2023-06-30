/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:28:44 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/30 16:54:52 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Function: redirection_file
** ---------------------------
** Determines if the next token in the list is an output or input file
** following a redirection.
**
** Args:
** - tmp: Pointer to the token list.
**
** Side effects:
** Assigns a type to the next token in the list
** based on the current token's type.
*/
void	redirection_file(t_list *tmp)
{
	if (tmp->type == REDIRECT_OUTPUT && tmp->next->type == 0)
		tmp->next->type = OUTFILE;
	if (tmp->type == REDIRECT_INPUT && tmp->next->type == 0)
		tmp->next->type = INFILE;
}

/*
** Function: check_quoted_phrase
** -----------------------------
** Checks if a quoted token contains space. 
** If not, it is assigned type COMMANDE.
**
** Args:
** - tmp: Pointer to the token list.
**
** Side effects:
** Assigns a type to the token based on its content.
*/
void	check_quoted_phrase(t_list *tmp)
{
	int	i;

	i = 0;
	while (tmp->content[i])
	{
		if (tmp->content[i] == ' ')
			return ;
		i++;
	}
	tmp->type = COMMANDE;
}

/*
** Function: type_arithmetic
** --------------------------
** If a token's length > 1 and it contains '+', the function checks for
** '=' following '+' and assigns type ARITHMETIC_APPEND.
**
** Args:
** - tmp: Pointer to the token list.
**
** Side effects:
** Assigns a type to the token based on its content.
*/
void	type_arithmetic(t_list *tmp)
{
	int	i;

	i = 1;
	while (tmp->content[i])
	{
		if (tmp->content[i] == '+')
		{
			i++;
			if (tmp->content[i] == '=')
			{
				tmp->type = ARITHMETIC_APPEND;
				return ;
			}
		}
		i++;
	}
}

/*
** Function: assign_type
** ----------------------
** Assigns types to tokens in the list based on their content.
**
** Args:
** - data: Pointer to the main data structure.
**
** Side effects:
** Assigns types to the tokens in the list based on their content.
*/
void	assign_type(t_data *data)
/* gerer les $variable <-- important */
{
	t_list	*tmp;

	tmp = data->token_list;
	while (tmp != NULL)
	{
		if (ft_strlen(tmp->content) == 1)
			len_is_one(data, tmp);
		if (ft_strlen(tmp->content) == 2)
			len_is_two(tmp);
		if (tmp->type == REDIRECT_INPUT || tmp->type == REDIRECT_OUTPUT)
			redirection_file(tmp);
		if (tmp->content[0] == '$')
			type_dollar(data, tmp);
		if (tmp->content[0] == '-')
			type_option(tmp);
		if (tmp->type == SINGLE_QUOTE || tmp->type == DOUBLE_QUOTE)
			check_quoted_phrase(tmp);
		if (ft_strlen(tmp->content) > 1 && tmp->type == 0)
			type_arithmetic(tmp);
		tmp = tmp->next;
	}
}
