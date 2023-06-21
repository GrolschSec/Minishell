/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_eof.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:07:54 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/21 18:56:52 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	count_heredoc(t_data *data)
// {
// 	int		x;
// 	t_list	*tmp;

// 	tmp = data->token_list;
// 	x = 0;
// 	while (tmp)
// 	{
// 		while (tmp != NULL && tmp->type != PIPE)
// 		{
// 			if (tmp->type == ENDOFFILE)
// 				data->exec[x].heredoc++;
// 			tmp = tmp->next;
// 		}
// 		if (tmp != NULL && tmp->type == PIPE)
// 		{
// 			tmp = tmp->next;
// 			x++;
// 		}
// 		else
// 			return ;
// 	}
// }

void	put_eofs_in_tab(t_data *data, t_list *tmp, t_exec *current)
{
	int	nb_eof;

	nb_eof = 0;
	if (current->heredoc <= 0)
		return ;
	else
		current->eof = malloc(sizeof(char *) * (current->heredoc + 1));
	if (current->eof == NULL)
		exit_all(data, 1, "problem in redirect input");
	while (tmp != NULL && nb_eof < current->heredoc)
	{
		if (tmp->type == ENDOFFILE)
		{
			current->eof[nb_eof] = ft_strdup(tmp->content);
			nb_eof++;
		}
		tmp = tmp->next;
	}
	current->eof[nb_eof] = NULL;
}

void	fill_eof(t_data *data)
{
	t_list	*tmp;
	int		x;

	x = 0;
	tmp = data->token_list;
	// count_heredoc(data);
	while (x < data->pipes)
	{
		put_eofs_in_tab(data, tmp, &data->exec[x]);
		if (tmp->type == PIPE && tmp != NULL)
			tmp = tmp->next;
		x++;
	}
}
