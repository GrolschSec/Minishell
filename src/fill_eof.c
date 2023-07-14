/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_eof.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:07:54 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/14 18:48:49 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	put_eofs_in_list(t_data *data, t_list **tmp, t_exec *current)
{
	int		nb_eof;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	nb_eof = 0;
	if (current->heredoc <= 0)
		return ;
	while ((*tmp) != NULL && nb_eof < current->heredoc)
	{
		if ((*tmp)->type == ENDOFFILE || (*tmp)->type == ENDOFFILE_QUOTED)
		{
			new = ft_lstnew((*tmp)->content);
			new->type = (*tmp)->type;
			ret = ft_lstadd_back(&current->eof, new);
			if (ret == 1)
				exit_all(data, 1, "Problem when add_back in outfile list");
			nb_eof++;
		}
		(*tmp) = (*tmp)->next;
	}
}

void	fill_eof(t_data *data, int nb)
{
	t_list	*tmp;
	int		x;

	x = 0;
	tmp = data->token_list;
	while (x < nb && tmp)
	{
		put_eofs_in_list(data, &tmp, &data->exec[x]);
		if (tmp && tmp->type == PIPE)
			tmp = tmp->next;
		x++;
	}
}
