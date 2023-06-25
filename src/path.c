/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:05:25 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/25 17:46:42 by mrabourd         ###   ########.fr       */
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

/*
** parse_path: Retrieves the PATH environment variable from the environment 
** list and stores its value in the data structure. Also splits the PATH 
** into a string array using ':' as the delimiter.
**
** Args:
**   data: Pointer to the data structure where the PATH value and the split 
**   PATH are stored.
**
** Side effects:
**   If an error occurs, the program exits with an error message.
**   If the PATH variable is found, its value is stored in data->path.line
**   and its split form is stored in data->path.tab.
*/
void	parse_path(t_data *data)
{
	t_list	*tmp;

	tmp = data->env;
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
/*	print_path(data);*/
}
