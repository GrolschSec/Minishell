/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 12:36:27 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/04 13:47:39 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	is_env_variable(t_data *data, t_list *tmp, int *i, char *prev)
{
	char	*variable;

	variable = ft_getenv(data, &tmp->content[*i]);
	if (variable == NULL && tmp->type == SINGLE_QUOTE)
		tmp->type = COMMANDE;
	else if (variable == NULL && (tmp->type == DOUBLE_QUOTE || tmp->type == 0))
	{
		free(tmp->content);
		tmp->content = malloc (sizeof(char) * 2);
		tmp->content[0] = ' ';
		tmp->content[1] = '\0';
		if (tmp->type == 0)
			data->error = 1;
	}
	else
	{
		free(tmp->content);
		if (prev != NULL)
			tmp->content = ft_strjoin(prev, variable);
		else
			tmp->content = ft_strdup(variable);
		free(variable);
	}
}

void	fill_exitcode(t_list *tmp, char *variable, char *prev, char *next)
{
	free(tmp->content);
	if (prev != NULL)
		tmp->content = ft_strjoin(prev, variable);
	else
		tmp->content = ft_strdup(variable);
	free (variable);
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
	variable = ft_itoa(*data->exit_code);
	fill_exitcode(tmp, variable, prev, next);
}

void	type_dollar(t_data *data, t_list *tmp, int i)
{
	char	*prev;

	prev = NULL;
	i++;
	if (tmp->content[i] == '\0')
		tmp->type = COMMANDE;
	else
	{
		if (i > 1)
		{
			prev = malloc (sizeof(char) * (i));
			if (prev == NULL)
				exit_all(data, 1, "Problem of malloc in dollar");
			ft_strlcpy(prev, tmp->content, i);
		}
		if (tmp->content[i] == '?')
			dollar_exitcode(data, tmp, &i, prev);
		else
			is_env_variable(data, tmp, &i, prev);
	}
}
