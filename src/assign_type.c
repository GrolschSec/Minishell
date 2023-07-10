/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:28:44 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/09 14:16:14 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	is_unexpected(t_data *data, t_list *tmp)
{
	if (tmp->content[0] == '&')
		tmp->type = EPERLUETTE;
	if (tmp->content[0] == '(' || tmp->content[0] == ')')
		tmp->type = PARENTHESIS;
	if (tmp->content[0] == ';')
		tmp->type = SEMICOLON;
	if (tmp->content[0] == ' ')
		tmp->type = BLANCK;
	if (tmp->type == EPERLUETTE
		|| tmp->type == PARENTHESIS
		|| tmp->type == SEMICOLON)
	{
		data->error = 1;
		g_exit = 2;
		printf("minishell: unexpected input\n");
	}
}

void	verify_dollar(t_data *data, t_list *tmp)
{
	int	i;

	i = 0;
	if (tmp == NULL || tmp->content[i] == '\0')
		return ;
	while (tmp->content[i])
	{
		if (tmp->content[i] == '$' && data->error == 0)
		{
			if (tmp->type != SINGLE_QUOTE)
			{
				type_dollar(data, tmp, i);
			}
		}
		if (tmp->content[i])
			i++;
	}
}

void	assign_type(t_data *data)
{
	t_list	*tmp;

	tmp = data->token_list;
	while (tmp != NULL)
	{
		if (tmp->content[0] == '~' && data->error == 0)
		{
			free(tmp->content);
			tmp->content = ft_strdup(data->tilde);
		}
		if (is_meta(tmp->content[0]) == 1 && data->error == 0)
			parse_meta(data, tmp);
		if (tmp->content[0] == '-' && data->error == 0)
			type_option(tmp);
		if (ft_strlen(tmp->content) > 1 && tmp->type == 0 && data->error == 0)
			type_arithmetic(tmp);
		verify_dollar(data, tmp);
		tmp = tmp->next;
	}
}
