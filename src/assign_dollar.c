/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 12:36:27 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/11 16:23:23 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*fill_next(t_data *data, t_list *tmp, int *i)
{
	char	*next;

	(void)data;
	next = NULL;
	(*i)--;
	if (*i == (int)ft_strlen(tmp->content) - 1)
		return (NULL);
	if (*i < ((int)ft_strlen(tmp->content) - 1))
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

void	is_env_variable(t_data *data, t_list *tmp, int *i, char *prev)
{
	char	*variable;
	char	*next;
	char	*var;

	var = parse_var(tmp->content, i);
	variable = ft_getenv(data, var);
	free(var);
	next = fill_next(data, tmp, i);
	if (tmp->type == SINGLE_QUOTE)
		tmp->type = COMMANDE;
	else if (variable == NULL)
	{
		free (tmp->content);
		if (prev)
		{
			tmp->content = ft_strdup(prev);
			free(prev);
		}
		else
			tmp->content = ft_strdup(" ");
	}
	else
	{
		fill_all(tmp, variable, prev, next);
	}
}

void	type_dollar(t_data *data, t_list *tmp, int i)
{
	char	*prev;

	prev = NULL;
	i++;
	if (tmp->content[i] == '\0')
		tmp->type = COMMANDE;
	if (tmp->type == ENDOFFILE)
		return ;
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
