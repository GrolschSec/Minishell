/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_dollar_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:55:22 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/19 19:46:16 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*fill_next(t_list *tmp, int *i)
{
	char	*next;

	next = NULL;
	if (next)
		free (next);
	(*i)--;
	if (*i == (int)ft_strlen(tmp->content) - 1)
		return (NULL);
	if (*i < ((int)ft_strlen(tmp->content)))
	{
		next = ft_substr(tmp->content, (*i + 1),
				ft_strlen(tmp->content) - (*i + 1));
	}
	return (next);
}

char	*parse_var(char *str, int *i)
{
	char	*var;
	int		y;

	y = *i;
	var = NULL;
	while (is_space(str[*i]) == 0 && (ft_isalnum(str[*i])
			|| str[*i] == '_'))
		(*i)++;
	var = ft_substr(str, y, *i - y);
	return (var);
}

int	check_variable(char *variable)
{
	int	i;

	i = 0;
	while (variable[i])
	{
		if (variable[i] != ' ')
			return (i);
		i++;
	}
	if ((int)ft_strlen(variable) == i)
		return (-1);
	return (i);
}

void	parse_var_env(t_data *data, t_list *tmp, char *variable)
{
	int	i;

	i = 0;
	if (!variable)
	{
		variable = NULL;
		return ;
	}
	tmp->var_env = malloc(sizeof(t_var_env));
	if (!tmp->var_env)
		exit_all(data, 1, "Malloc problem in var env");
	tmp->var_env->name = ft_strdup(tmp->content);
	if (check_variable(variable) == -1)
	{
		tmp->var_env->value = malloc_one_tab(data, tmp->var_env->value);
		tmp->var_env->nb_value = 1;
		return ;
	}
	tmp->var_env->value = ft_split(variable, ' ');
	while (tmp->var_env->value[i] != NULL)
		i++;
	tmp->var_env->nb_value = i;
}

void	is_env_variable(t_data *data, t_list *tmp, int *i, char *prev)
{
	char	*variable;
	char	*next;
	char	*var;

	var = parse_var(tmp->content, i);
	if (ft_strlen(var) == 0)
	{
		free(var);
		return ;
	}
	variable = ft_getenv(data, var);
	parse_var_env(data, tmp, variable);
	free(var);
	next = fill_next(tmp, i);
	if (tmp->type == SINGLE_QUOTE)
		tmp->type = COMMANDE;
	if (!variable && !next && !prev)
		malloc_one(data, tmp);
	else
	{
		fill_all(tmp, variable, prev, next);
	}
}
