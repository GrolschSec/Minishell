/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 19:45:47 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/19 19:46:22 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**malloc_one_tab(t_data *data, char **str)
{
	str = malloc(sizeof(char *) * 2);
	if (!str)
		exit_all(data, 1, "Malloc problem in malloc_one_tab");
	str[0] = malloc(sizeof(char) * 2);
	if (!str[0])
		exit_all(data, 1, "Malloc problem in malloc_one_tab");
	str[0][0] = ' ';
	str[0][1] = '\0';
	str[1] = NULL;
	return (str);
}

void	malloc_one(t_data *data, t_list *tmp)
{
	tmp->content = malloc(sizeof(char));
	if (!tmp->content)
		exit_all(data, 1, "Malloc problem in malloc_one");
	tmp->content[0] = '\0';
}
