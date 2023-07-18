/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:33:03 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/18 21:04:47 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free (tab[i]);
		i++;
	}
	free (tab);
}

void	free_token_list(t_data *data)
{
	t_list	*tmp;

	tmp = data->token_list;
	while (tmp != NULL)
	{
		if (tmp->var_env)
		{
			free_tab(tmp->var_env->value);
			free(tmp->var_env->name);
			free(tmp->var_env);
			tmp->var_env = NULL;
		}
		tmp = tmp->next;
	}
	ft_lstclear(&data->token_list, del);
}

void	clear_cmd(t_data *data)
{
	int		i;

	i = 0;
	free_token_list(data);
	if (data->pipes > 0)
	{
		while (i < data->pipes)
		{
			if (data->exec[i].cmd && data->exec[i].cmd != NULL)
				free_tab(data->exec[i].cmd);
			if (data->exec[i].infile && data->exec[i].infile != NULL)
				free_tab(data->exec[i].infile);
			if (data->exec[i].outfile && data->exec[i].outfile != NULL)
			{
				ft_lstclear(&data->exec[i].outfile, del);
				free(data->exec[i].outfile);
			}
			if (data->exec[i].eof && data->exec[i].eof != NULL)
			{
				ft_lstclear(&data->exec[i].eof, del);
				free(data->exec[i].eof);
			}
			i++;
		}
		if (data->exec && data->exec != NULL)
			free(data->exec);
	}
}

void	free_env(t_data *data)
{
	if (data->env && data->env != NULL)
		ft_lstclear(&data->env, del);
}

void	exit_all(t_data *data, int err, char *str)
{
	data->error = 1;
	if (str != NULL)
		printf("%s\n", str);
	free_env(data);
	if (str != NULL)
		printf("%s\n", str);
	if (data->tilde)
		free(data->tilde);
	if (err != 0)
	{
		clear_cmd(data);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
