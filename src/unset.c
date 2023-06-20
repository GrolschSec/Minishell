/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 15:33:34 by mrabourd          #+#    #+#             */
/*   Updated: 2023/05/24 16:26:29 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	first_variable_unset(t_data *data, t_list *node, char *var, t_list *tmp)
{
	node = tmp;
	data->env = data->env->next;
	ft_lstdelone(node, free);
	free (var);
	return ;
}

void	unset_variables(t_list *node, char *var, t_list *tmp, t_list *to_unset)
{
	int	len_variable;
	int	len_to_unset;

	len_variable = ft_strlen(var);
	len_to_unset = ft_strlen(to_unset->content);
	if (len_to_unset > len_variable)
		return ;
	if (ft_strncmp(to_unset->content, var, len_variable) == 0)
	{
		node = tmp->next;
		tmp->next = tmp->next->next;
		ft_lstdelone(node, free);
		return ;
	}
}

void	builtin_unset(t_data *data, t_list *to_unset)
{
	t_list	*tmp;
	char	*var;
	t_list	*node;

	var = NULL;
	tmp = data->env;
	node = NULL;
	if (to_unset->next != NULL)
		to_unset = to_unset->next;
	else
		exit_all(data, 1, "There is one argument missing to unset");
	var = extract_name(data, tmp);
	if (ft_strncmp(to_unset->content, var, ft_strlen(var)) == 0)
		first_variable_unset(data, node, var, tmp);
	free (var);
	while (tmp != NULL)
	{
		var = extract_name(data, tmp->next);
		unset_variables(node, var, tmp, to_unset);
		free(var);
		tmp = tmp->next;
		if (tmp->next == NULL)
			return ;
	}
}
