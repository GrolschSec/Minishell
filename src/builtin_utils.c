/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 16:48:42 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/06/30 17:08:49 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_value(t_list *env)
{
	char	**split;
	char	*tmp;

	split = ft_split(env->content, '=');
	if (!split)
		return (NULL);
	if (!split[1])
		return (free_tab(split), NULL);
	tmp = split[1];
	return (free(split[0]), free(split), tmp);
}

char	*ft_getenv(t_data *data, char *name)
{
	t_list	*tmp;
	char	*str_tmp;

	str_tmp = ft_strjoin2(name, "=");
	if (!str_tmp)
		return (NULL);
	tmp = data->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, str_tmp, ft_strlen(str_tmp)) == 0)
			break ;
		tmp = tmp->next;
	}
	free(str_tmp);
	str_tmp = NULL;
	if (tmp)
	{
		str_tmp = get_value(tmp);
		if (!str_tmp)
			return (NULL);
		return (str_tmp);
	}
	return (NULL);
}
