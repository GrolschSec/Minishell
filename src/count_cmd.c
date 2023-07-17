/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 01:50:06 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/16 19:25:14 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	count_cmd_and_redir(t_data *data)
{
	t_list	*tmp;
	int		x;

	x = 0;
	tmp = data->token_list;
	while (x < data->pipes)
	{
		while (tmp != NULL && tmp->type != PIPE)
		{
			if (tmp->type == REDIRECT_INPUT)
				data->exec[x].redirect_input++;
			else if (tmp->type == REDIRECT_OUTPUT || tmp->type == DELIM_APPEND)
				data->exec[x].redirect_output++;
			else if (tmp->type == HEREDOC)
				data->exec[x].heredoc++;
			else if (is_not_redirection(tmp) && tmp->type != PIPE)
			{
				if (tmp->var_env && tmp->var_env->nb_value)
					data->exec[x].nb_cmd += tmp->var_env->nb_value;
				else
					data->exec[x].nb_cmd++;
			}
			tmp = tmp->next;
		}
		x++;
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
		x++;
	}
}

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
	malloc_init_exec(data);
	count_cmd_and_redir(data);
	which_is_last_infile(data);
}
