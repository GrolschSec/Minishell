/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 01:50:06 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/12 16:54:33 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Function: is_not_redirection
** ----------------------------
** Determines if a token is not a redirection.
**
** Args:
** - tmp: Pointer to the token list.
**
** Returns:
** - 1 if the token is not a redirection, 0 otherwise.
*/
int	is_not_redirection(t_list *tmp)
{
	if (tmp->type != INFILE && tmp->type != OUTFILE
		&& tmp->type != REDIRECT_INPUT && tmp->type != REDIRECT_OUTPUT
		&& tmp->type != HEREDOC && tmp->type != ENDOFFILE
		&& tmp->type != DELIMITER_APPEND)
		return (1);
	return (0);
}

/*
** Function: count_pipes
** ---------------------
** Counts the number of PIPE tokens in a list.
**
** Args:
** - data: Pointer to the main data structure.
**
** Side effects:
** - The number of PIPE tokens found is stored in data->pipes.
*/
void	count_pipes(t_data *data)
{
	t_list	*tmp;

	tmp = data->token_list;
	data->pipes = 0;
	while (tmp != NULL)
	{
		if (tmp->type == PIPE)
			data->pipes++;
		tmp = tmp->next;
	}
	data->pipes++;
}

/*
** Function: count_redirections
** ----------------------------
** Counts the number of redirections in a command.
**
** Args:
** - tmp: Pointer to the token list.
** - current: Pointer to the current command structure.
** - x: Index of the command structure.
**
** Side effects:
** - Updates the redirection count in the command structure.
*/
void	count_cmd_and_redir(t_list **tmp, t_exec *current, int x)
{
	while ((*tmp) != NULL && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == REDIRECT_INPUT)
			current[x].redirect_input++;
		else if ((*tmp)->type == REDIRECT_OUTPUT || (*tmp)->type == DELIMITER_APPEND)
			current[x].redirect_output++;
		else if ((*tmp)->type == HEREDOC)
			current[x].heredoc++;
		else if (is_not_redirection(*tmp) && (*tmp)->type != PIPE)
			current[x].nb_cmd++;
		(*tmp) = (*tmp)->next;
	}
	if ((*tmp) != NULL && (*tmp)->type == PIPE)
		(*tmp) = (*tmp)->next;
	else if ((*tmp) == NULL)
		return ;
}

/*
** Function: init_exec
** -------------------
** Initializes an 't_exec' structure.
**
** Args:
** - current: Pointer to the 't_exec' structure to be initialized.
** - x: Index of the 't_exec' structure.
**
** Side effects:
** - Initializes the 't_exec' structure to default values.
*/
void	init_exec(t_exec *current, int x)
{
	current[x].cmd = NULL;
	current[x].infile = NULL;
	current[x].outfile = NULL;
	current[x].eof = NULL;
	current[x].fdin = 0;
	current[x].fdout = 1;
	current[x].redirect_input = 0;
	current[x].redirect_output = 0;
	current[x].heredoc = 0;
	current[x].is_last = 0;
	current[x].nb_cmd = 0;
}
