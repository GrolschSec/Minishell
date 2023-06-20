/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:05:25 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 15:24:00 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
/*
void	print_path(t_data *data)
{
	printf("path: %s\n", data->path.line);
	printf("tab0: %s\n", data->path.tab[0]);
}
*/
void	parse_path(t_data *data)
{
	t_list	*tmp;

	tmp = data->env;
	// printf("nb_env dans path debut: %d\n", data->nb_env);
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->content, "PATH=", 5) == 0)
			data->path.line = ft_strdup(&tmp->content[5]);
		tmp = tmp->next;
	}
	if (data->path.line)
		data->path.tab = ft_split(data->path.line, ':');
	else
		exit_all(data, 1, "env: No such file or directory");
	// printf("nb_env dans path FIN : %d\n", data->nb_env);
/*	print_path(data);*/
}
