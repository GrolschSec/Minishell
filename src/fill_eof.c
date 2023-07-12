/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_eof.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:07:54 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/12 18:31:42 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	fill_eof(t_data *data, int nb)
{
	t_list	*tmp;
	int		x;

	x = 0;
	tmp = data->token_list;
	// data->exec = malloc(sizeof(t_exec) * nb);
	// if (data->exec == NULL)
	// 	exit_all(data, 1, "malloc probleme pour structure");
	// while (x < nb)
	// {
	// 	init_exec(data->exec, x);
	// 	count_cmd_and_redir(&tmp, data->exec, x);
	// 	x++;
	// }
	// x = 0;
	// tmp = data->token_list;
	while (x < nb)
	{
		put_eofs_in_tab(data, tmp, &data->exec[x]);
		if (tmp->type == PIPE && tmp->next != NULL)
			tmp = tmp->next;
		x++;
	}
}
