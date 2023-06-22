/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 01:50:06 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/22 19:23:19 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_not_redirection(t_list *tmp)
{
	if (tmp->type != INFILE && tmp->type != OUTFILE
		&& tmp->type != REDIRECT_INPUT && tmp->type != REDIRECT_OUTPUT
		&& tmp->type != HEREDOC && tmp->type != ENDOFFILE
		&& tmp->type != DELIMITER_APPEND)
		return (1);
	return (0);
}

int	count_cmd(t_list *tmp)
{
	int		i;

	i = 0;
	while (tmp != NULL)
	{
		if (is_not_redirection(tmp) && tmp->type != PIPE)
			i++;
		if (tmp->type == PIPE)
		{
			return (i);
		}
		tmp = tmp->next;
	}
	return (i);
}

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

void	count_redirections(t_list *tmp, t_exec *current, int x)
{
	if (tmp->type == REDIRECT_INPUT)
		current[x].redirect_input++;
	if (tmp->type == REDIRECT_OUTPUT || tmp->type == DELIMITER_APPEND)
	{
		current[x].redirect_output++;
		if (current[x].last_redir_out != NULL)
			free(current[x].last_redir_out);
		current[x].last_redir_out = ft_strdup(tmp->content);
	}
	if (tmp->type == HEREDOC)
		current[x].heredoc++;
	// if (tmp->type == DELIMITER_APPEND)
	// 	current[x].delimiter_append++;
}

void	init_exec(t_exec *current, int x)
{
	current[x].infile = NULL;
	current[x].outfile = NULL;
	current[x].eof = NULL;
	current[x].last_redir_out = NULL;
	current[x].fdin = 0;
	current[x].fdout = 1;
	current[x].redirect_input = 0;
	current[x].redirect_output = 0;
	current[x].heredoc = 0;
	// current[x].delimiter_append = 0;
}
