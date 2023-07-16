/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_dollar_exitcode.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 16:24:57 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/16 21:41:45 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fill_all(t_list *tmp, char *variable, char *prev, char *next)
{
	free(tmp->content);
	if (prev && variable)
	{
		tmp->content = ft_strjoin(prev, variable);
		free(variable);
	}
	else if (prev && !variable)
	{
		tmp->content = ft_strdup(prev);
		free(prev);
	}
	else if (variable && !prev)
	{
		tmp->content = ft_strdup(variable);
		free (variable);
	}
	else if (!prev && !variable)
		tmp->content = ft_strdup(" ");
	if (next != NULL)
	{
		tmp->content = ft_strjoin(tmp->content, next);
		free (next);
	}
	if ((variable && prev) || (variable && next))
	{
		free_tab(tmp->var_env->value);
		free(tmp->var_env->name);
		tmp->var_env->nb_value = 0;
		tmp->var_env = NULL;
	}
}

void	dollar_exitcode(t_data *data, t_list *tmp, int *i, char *prev)
{
	char	*variable;
	char	*next;

	(void)data;
	next = NULL;
	variable = NULL;
	if (*i < ((int)ft_strlen(tmp->content) - 1))
	{
		next = ft_substr(tmp->content, (*i + 1),
				ft_strlen(tmp->content) - (*i + 1));
	}
	variable = ft_itoa(g_exit);
	fill_all(tmp, variable, prev, next);
}
