/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:17:23 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/16 21:17:31 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	del(void *list)
{
	t_list	*tmp;

	tmp = (t_list *)list;
	free (tmp->content);
	tmp->var_env = NULL;
	tmp->content = NULL;
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
