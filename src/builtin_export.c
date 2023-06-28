/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:12:49 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/06/28 13:03:39 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*Comportement export*/

void	update_env(t_data *data, char *name, char *value)
{
	t_list	*tmp;
	t_list	*env;
	char	*tmp_str;
	char	*new_env;

	env = NULL;
	tmp = data->env;
	new_env = make_new_var(name, value);
	while (tmp)
	{
		if (ft_strncmp(name, tmp->content, ft_strlen(name)) == 0)
		{
			env = tmp;
			break ;
		}
		tmp = tmp->next;
	}
	if (!env)
		ft_lstadd_back(&data->env, ft_lstnew(new_env));
	else
	{
		tmp_str = env->content;
		env->content = new_env;
		free(tmp_str);
	}
}

char	*make_new_var(char *name, char *value)
{
	char	*tmp_str;
	char	*new_env;

	tmp_str = ft_strjoin2(name, "=");
	if (!tmp_str)
		return (free(value), NULL);
	new_env = ft_strjoin(tmp_str, value);
	if (!new_env)
		return (free(value), NULL);
	return (free(value), new_env);
}
