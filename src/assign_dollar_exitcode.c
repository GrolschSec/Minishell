/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_dollar_exitcode.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 16:24:57 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/19 20:54:56 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_before_fill(t_list *tmp, char *variable, char *prev, char *next)
{
	if (variable && (prev || next))
	{
		if (tmp->var_env && tmp->var_env->nb_value > 0)
		{
			free_tab(tmp->var_env->value);
			free(tmp->var_env->name);
			free(tmp->var_env);
			tmp->var_env = NULL;
		}
	}
	free(tmp->content);
}

void	fill_all(t_list *tmp, char *variable, char *prev, char *next)
{
	free_before_fill(tmp, variable, prev, next);
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
