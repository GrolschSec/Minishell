/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 15:22:50 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/11 15:47:24 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	unset_builtin(t_data *data, t_exec *exec)
{
	int	i;

	i = 1;
	while (i < exec->nb_cmd)
	{
		if (data->pipes == 1 && exec->is_last)
			ft_unsetenv(data, exec->cmd[i]);
		i++;
	}
	g_exit = 0;
}

void	del_first_node(t_data *data)
{
	t_list	*tmp;

	tmp = data->env;
	if (!tmp)
		return ;
	data->env = data->env->next;
	free(tmp->content);
	free(tmp);
}

void	del_next_node(t_list *prev)
{
	t_list	*tmp;
	t_list	*next;

	next = NULL;
	if (!prev)
		return ;
	tmp = prev->next;
	if (tmp)
		next = tmp->next;
	if (next)
		prev->next = next;
	if (tmp)
	{
		free(tmp->content);
		free(tmp);
	}
}

void	check_next_env(t_list *current, t_list *prev, char *str)
{
	while (current)
	{
		if (current->content
			&& ft_strncmp(current->content, str, ft_strlen(str)) == 0)
		{
			prev->next = current->next;
			free(current->content);
			free(current);
			current = prev->next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void	ft_unsetenv(t_data *data, char *name)
{
	char	*str;
	t_list	*current;
	t_list	*prev;

	str = ft_strjoin2(name, "=");
	if (!str)
		return ;
	current = data->env;
	if (current && ft_strncmp(str, current->content, ft_strlen(str)) == 0)
	{
		del_first_node(data);
		free(str);
		return ;
	}
	prev = current;
	current = current->next;
	check_next_env(current, prev, str);
	free(str);
	return ;
}
