/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 18:12:25 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/16 15:05:09 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	redirection_file(t_data *data, t_list *tmp)
{
	if ((tmp->type == REDIRECT_INPUT || tmp->type == REDIRECT_OUTPUT
			|| tmp->type == HEREDOC || tmp->type == DELIM_APPEND)
		&& tmp->next == NULL )
	{
		error(data, "`newline'");
	}
	else if ((tmp->type == REDIRECT_OUTPUT || tmp->type == DELIM_APPEND)
		&& tmp->next->type != PIPE)
	{
		if (is_meta(tmp->next->content[0]) == 0)
			tmp->next->type = OUTFILE;
		else
			error(data, "`>'");
	}
	else if (tmp->type == REDIRECT_INPUT && tmp->next->type != PIPE)
	{
		if (is_meta(tmp->next->content[0]) == 0)
			tmp->next->type = INFILE;
		else
			error(data, "`<'");
	}
}

void	heredoc_type(t_data *data, t_list *tmp)
{
	if (tmp->next != NULL)
	{
		if (is_meta(tmp->next->content[0]) == 0 && (tmp->next->type == 0))
			tmp->next->type = ENDOFFILE;
		else if (tmp->next->type == SINGLE_QUOTE
			|| tmp->next->type == DOUBLE_QUOTE)
			tmp->next->type = ENDOFFILE_QUOTED;
		else
			error(data, "`<<'");
	}
	else
		error(data, "`newline'");
}

void	is_redir_input(t_data *data, t_list *tmp, int i)
{
	if (ft_strlen(tmp->content) == 1)
	{
		tmp->type = REDIRECT_INPUT;
		redirection_file(data, tmp);
	}
	else if (ft_strlen(tmp->content) == 2)
	{
		tmp->type = HEREDOC;
		heredoc_type(data, tmp);
	}
	if (i != 0 && i != 1)
		error (data, "`<<'");
}

void	is_redir_output(t_data *data, t_list *tmp, int i)
{
	if (ft_strlen(tmp->content) == 1)
		tmp->type = REDIRECT_OUTPUT;
	else if (ft_strlen(tmp->content) == 2)
		tmp->type = DELIM_APPEND;
	redirection_file(data, tmp);
	if (i != 0 && i != 1)
		error (data, "`>>'");
}

void	parse_meta(t_data *data, t_list *tmp)
{
	int	i;
	int	y;

	y = (int)ft_strlen(tmp->content);
	i = 0;
	while (i + 1 <= y && tmp->content[i] == tmp->content[i + 1])
		i++;
	if (tmp->content[i] == '<')
		is_redir_input(data, tmp, i);
	else if (tmp->content[i] == '>')
		is_redir_output(data, tmp, i);
	else if (tmp->content[i] == '|')
	{
		if (i == 0)
			tmp->type = PIPE;
		else
			error (data, tmp->content);
	}
	else
		is_unexpected(data, tmp);
}
