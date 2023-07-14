/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 12:36:27 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/14 18:42:50 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*fill_next(t_list *tmp, int *i)
{
	char	*next;

	next = NULL;
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

void	is_env_variable(t_data *data, t_list *tmp, int *i, char *prev)
{
	char	*variable;
	char	*next;
	char	*var;

	var = parse_var(tmp->content, i);
	if (ft_strlen(var) == 0)
		return ;
	variable = ft_getenv(data, var);
	free(var);
	next = fill_next(tmp, i);
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
		if (next)
			free (next);
	}
	else
	{
		fill_all(tmp, variable, prev, next);
	}
}

int		dollar_in_single_quotes(t_list *tmp, int i)
{
	int	y;

	y = 0;
	if (i > 0)
	{
		while (tmp->content[i] != '\0')
		{
			if (tmp->content[i] == '\'')
				return (y);
			i++;
			y++;
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
	(*i)++;
	next = fill_next(tmp, i);
	if (variable == NULL)
	{
		free (tmp->content);
		if (prev)
		{
			tmp->content = ft_strdup(prev);
			tmp->content = ft_strjoin(tmp->content, "\'");
			free(prev);
			if (next)
				tmp->content = ft_strjoin(tmp->content, next);
		}
		else
			tmp->content = ft_strdup(" ");
	}
}

int		only_space(t_list *tmp, int i)
{
	while (i > 0)
	{
		if (tmp->content[i] != ' ')
			return (0);
		i--;
	}
	return (1);
}

void	type_dollar(t_data *data, t_list *tmp, int i)
{
	char	*prev;

	prev = NULL;
	i++;
	if (tmp->content[i] == '\0')
	{
		tmp->type = COMMANDE;
		return ;
	}
	if (tmp->type == ENDOFFILE || tmp->type == ENDOFFILE_QUOTED)
		return ;
	if (i > 1)
	{
		if (only_space(tmp, i) == 1)
		{
			prev = malloc (sizeof(char) * (i));
			if (prev == NULL)
				exit_all(data, 1, "Problem of malloc in dollar");
			ft_strlcpy(prev, tmp->content, i);
		}
	}
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
