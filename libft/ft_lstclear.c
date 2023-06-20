/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:17:23 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/06 00:41:19 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	del(void *list)
{
	t_list	*tmp;
	tmp = (t_list *)list;
	free (tmp->content);
	tmp->content = NULL;
	tmp->full = 0;
	tmp->printed = 0;
	tmp->type = 0;
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		del((*lst));
		free (*lst);
		*lst = tmp;
	}
}
