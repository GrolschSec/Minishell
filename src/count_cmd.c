/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 01:50:06 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/12 19:03:41 by mrabourd         ###   ########.fr       */
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
	current[x].is_eof = 0;
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
void	count_cmd_and_redir(t_data *data)
{
	t_list	*tmp;
	int		x;

	x = 0;
	tmp = data->token_list;
	data->exec = malloc(sizeof(t_exec) * data->pipes);
	if (data->exec == NULL)
		exit_all(data, 1, "malloc probleme pour structure");
	while (x < data->pipes)
	{
		init_exec(data->exec, x);
		while (tmp != NULL && tmp->type != PIPE)
		{
			if (tmp->type == REDIRECT_INPUT)
				data->exec[x].redirect_input++;
			else if (tmp->type == REDIRECT_OUTPUT || tmp->type == DELIMITER_APPEND)
				data->exec[x].redirect_output++;
			else if (tmp->type == HEREDOC)
				data->exec[x].heredoc++;
			else if (is_not_redirection(tmp) && tmp->type != PIPE)
				data->exec[x].nb_cmd++;
			tmp = tmp->next;
		}
		if (tmp != NULL && tmp->type == PIPE)
			tmp = tmp->next;
		else if (tmp == NULL)
			return ;
	}
}

void	which_infile(t_exec *current, t_list **tmp)
{
	int		y;
	int		z;

	z = 0;
	y = 0;
	while ((*tmp) != NULL && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == REDIRECT_INPUT)
			y++;
		if ((*tmp)->type == HEREDOC)
			z++;
		if (y == current->redirect_input && z != current->heredoc)
		{
			current->is_eof = 1;
			break ;
		}
		else if (y != current->redirect_input && z == current->heredoc)
		{
			current->is_eof = 0;
			break ;
		}
		(*tmp) = (*tmp)->next;
	}
	if ((*tmp) != NULL && (*tmp)->type == PIPE)
		(*tmp) = (*tmp)->next;
}

void	which_is_last_infile(t_data *data)
{
	t_list	*tmp;
	int		x;

	tmp = data->token_list;
	x = 0;
	while (x < data->pipes)
	{
		if (data->exec[x].heredoc == 0)
			data->exec[x].is_eof = 0;
		else
		{
			if (data->exec[x].redirect_input == 0)
				data->exec[x].is_eof = 1;
			else
				which_infile(&data->exec[x], &tmp);
		}
		printf("data->exec[x].is_eof: %d\n", data->exec[x].is_eof);
		x++;
	}
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
void	count_pipes_cmd_redir(t_data *data)
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
	count_cmd_and_redir(data);
	which_is_last_infile(data);
}
