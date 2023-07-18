/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 12:36:27 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/18 20:52:45 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	dollar_in_single_quotes(t_list *tmp, int i)
{
	if (i > 0)
	{
		while (tmp->content[i] != '\0')
		{
			if (tmp->content[i] == '\'')
				return (i);
			i++;
		}
	}
	return (0);
}

void	replace_dollar(t_data *data, t_list *tmp, int *i, char *prev)
{
	char	*variable;
	char	*next;
	char	*var;

	var = parse_var(tmp->content, i);
	variable = ft_getenv(data, var);
	free(var);
	next = fill_next(tmp, i);
	fill_all(tmp, variable, prev, next);
}
/*
int	only_space(t_list *tmp, int i)
{
	while (tmp->content[i] != '\0'
		|| tmp->content[i] != '\''
		|| tmp->content[i] != '\"')
	{
		if (tmp->content[i] != ' ')
			return (0);
		i--;
	}
	return (1);
}
*/

void	fill_prev(t_data *data, t_list *tmp, char **prev, int i)
{
	*prev = malloc (sizeof(char) * (i));
	if (*prev == NULL)
		exit_all(data, 1, "Problem of malloc in dollar");
	ft_strlcpy(*prev, tmp->content, i);
}

void	type_dollar(t_data *data, t_list *tmp, int i)
{
	char	*prev;

	prev = NULL;
	i++;
	if (tmp->type == ENDOFFILE || tmp->type == ENDOFFILE_QUOTED
		|| tmp->content[i] == '\0')
	{
		if (tmp->content[i] == '\0')
			tmp->type = COMMANDE;
		return ;
	}
	if (i > 1 && tmp->content[i] != '$')
		fill_prev(data, tmp, &prev, i);
	if (tmp->content[i] == '?')
		dollar_exitcode(data, tmp, &i, prev);
	else
	{
		if (dollar_in_single_quotes(tmp, i) != 0)
			replace_dollar(data, tmp, &i, prev);
		else
			is_env_variable(data, tmp, &i, prev);
	}
}
