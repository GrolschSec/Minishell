/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:00:03 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 14:47:15 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_strisalnumunderscore(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalpha(str[i]) == 0 && ft_isdigit(str[i]) == 0 && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	variable_already_exists(t_data *data, char *name)
{
	t_list	*check;

	check = data->env;
	while (check != NULL)
	{
		if (ft_strncmp(name, check->content, ft_strlen(name)) == 0)
			return (0);
		check = check->next;
	}
	return (1);
}

void	replace_value(t_data *data, char *variable, char *name)
{
	t_list	*to_replace;
	
	to_replace = data->env;
	while (ft_strncmp(name, to_replace->content,
			ft_strlen(to_replace->content)) != 0
		&& ft_strncmp(name, to_replace->content, ft_strlen(name)) != 0)
	{
		to_replace = to_replace->next;
	}
	free (to_replace->content);
	// if (variable->next != NULL && variable->next->type == VARIABLE_VALUE)
	// {
	// 	to_replace->content = ft_strdup(name);
	// 	to_replace->content = ft_strjoin(to_replace->content, "=");
	// 	to_replace->content = ft_strjoin(to_replace->content,
	// 			variable->next->content);
	// }
	// else
		to_replace->content = ft_strdup(variable);
}

char	*extract_name(t_data *data, char *variable)
{
	char	*name;
	int		i;
	int		dup;

	dup = 0;
	i = 0;
	name = NULL;
	while (variable[i] != '=')
		i++;
	name = malloc(sizeof(char) * (i + 1));
	if (name == NULL)
		exit_all(data, 1, "Problem of malloc when check variable (export)");
	while (dup < i)
	{
		name[dup] = variable[dup];
		dup++;
	}
	name[dup] = '\0';
	return (name);
}

void	do_export(t_data *data, char **pos)
{
	int		i;
	char 	*name;
	t_list	*new;
	int		ret;

	ret = 0;
	name = NULL;
	new = NULL;
	i = 1;
	if (ft_strchr(pos[i], '=') != 0)
	{
		name = extract_name(data, pos[i]);
		if (variable_already_exists(data, name) == 0)/* gerer leaks */
			replace_value(data, pos[i], name);
		else if ((pos[i][0] == '_' || ft_isalpha(pos[i][0]))
			&& ft_strisalnumunderscore(name))
		{
			new = ft_lstnew(pos[i]);
			new->type = ENVIRONMENT_VARIABLE;
			new->full = 0;
			// if (tmp->next && tmp->next->type == VARIABLE_VALUE)
			// 	new->content = ft_strjoin(new->content, tmp->next->content);
			ret = ft_lstadd_back(&data->env, new);
			if (ret == 1)
				exit_all(data, 1, "Problem of malloc when add a variable (export)");
		}
		free (name);
	}
}

void	builtin_export(t_data *data, char **pos)
{
	int	i;

	i = 0;
	if (!data->env)
		exit(EXIT_FAILURE);
	if (pos[i + 1])
	{
		do_export(data, pos);
	}
	else
		return ;
}
