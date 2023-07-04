/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_type_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 18:12:25 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/03 15:57:34 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	replace_by_tilde(t_data *data, t_list *tmp)
{
	free(tmp->content);
	tmp->content = ft_strdup(data->tilde);
}

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
		tmp->type = EPERLUETTE;
	if ((tmp->content[0] == '(' || tmp->content[0] == ')') && tmp->type == 0)
		tmp->type = PARENTHESIS;
	if (tmp->content[0] == ';' && tmp->type == 0)
		tmp->type = SEMICOLON;
	if (tmp->content[0] == ' ' && tmp->type == 0)
		tmp->type = BLANCK;
	if (tmp->content[0] == '~')
		replace_by_tilde(data, tmp);
	if (tmp->type == EPERLUETTE || tmp->type == PARENTHESIS
		|| tmp->type == SEMICOLON)
	{
		data->error = 1;
		*data->exit_code = 2;
		printf("minishell: unexpected input\n");
	}
}

void	heredoc_type(t_data *data, t_list *tmp)
{
	tmp->type = HEREDOC;
	if (tmp->next != NULL)
	{
		if (is_meta(tmp->next->content[0]) == 0)
			tmp->next->type = ENDOFFILE;
		else
		{
			data->error = 1;
			printf("minishell: syntax error near unexpected token `<<'\n");
		}
	}
	else
	{
		data->error = 1;
		printf("minishell: syntax error near unexpected token `newline'\n");
	}
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
void	len_is_two(t_data *data, t_list *tmp)
{
	if (tmp->content[0] == '<' && tmp->content[1] == '<' )
		heredoc_type(data, tmp);
	if (tmp->content[0] == '>' && tmp->content[1] == '>')
	{
		tmp->type = DELIMITER_APPEND;
		if (tmp->next != NULL)
		{
			if (is_meta(tmp->next->content[0]) == 0)
				tmp->next->type = OUTFILE;
			else
			{
				data->error = 1;
				printf("minishell: syntax error near unexpected token `>>'\n");
			}
		}
		else
		{
			data->error = 1;
			printf("minishell: syntax error near unexpected token `newline'\n");
		}
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
