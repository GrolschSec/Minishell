/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:28:44 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/23 17:19:20 by rlouvrie         ###   ########.fr       */
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
	if (tmp->content[0] == ' ' && tmp->type == 0) /*peut etre useless celui la*/
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
void	type_dollar(t_list *tmp)
{
	int	i;

	i = 1;
	while (tmp->content[i])
	{
		if (ft_strlen(tmp->content) == 2 && tmp->content[i] == '?')
			tmp->type = EXIT_STATUS;
		else
			tmp->type = ENVIRONMENT_VARIABLE;
		i++;
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
** Assigns a type to the next token in the list based on the current token's type.
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
** Checks if a quoted token contains space. If not, it is assigned type COMMANDE.
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
	tmp->type = COMMANDE; /* rechecker ce que ca peut etre */
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
void	assign_type(t_data *data)/* gerer rapidement les $variable <-- important */
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
			type_dollar(tmp);
		if (tmp->content[0] == '-')
			type_option(tmp);
		if (tmp->type == SINGLE_QUOTE || tmp->type == DOUBLE_QUOTE) /* pas forcement la meilleure facon de faire, a voir */
			check_quoted_phrase(tmp);
		if (ft_strlen(tmp->content) > 1 && tmp->type == 0)
			type_arithmetic(tmp);
		// if (ft_strncmp(tmp->content, "export", ft_strlen(tmp->content)) == 0)
		// 	assign_variable_name(data, tmp);
		tmp = tmp->next;
	}
}
