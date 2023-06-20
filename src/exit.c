/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:33:03 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 15:24:39 by mrabourd         ###   ########.fr       */
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

void	clear_cmd(t_data *data)
{
	int		i;

	i = 0;
	if (data->token_list && data->token_list != NULL)
		ft_lstclear(&data->token_list, del);
	if (data->pipes > 0)
	{
		while (i < data->pipes)
		{
			if (data->exec[i].cmd)
				free_tab(data->exec[i].cmd);
			if (data->exec[i].infile)
				free_tab(data->exec[i].infile);
			if (data->exec[i].outfile)
				free_tab(data->exec[i].outfile);
			data->exec[i].fdin = 0;
			data->exec[i].fdout = 1;
			data->exec[i].redirect_input = 0;
			data->exec[i].redirect_output = 0;
			data->exec[i].heredoc = 0;
			data->exec[i].delimiter_append = 0;
			i++;
		}
	}
}

void	free_env(t_data *data)
{
	if (data->env && data->env != NULL)
		ft_lstclear(&data->env, del);
	if (data->env_tab && data->env_tab != NULL)
		free_tab(data->env_tab);
}

void	exit_all(t_data *data, int err, char *str)
{
	if (str != NULL)
		fprintf(stderr, "%s\n", str);
	free_env(data);
	if (data->path.tab && data->path.tab != NULL)
		free_tab(data->path.tab);
	if (data->path.line && data->path.line != NULL)
		free (data->path.line);
	if (err == 1)
	{
		clear_cmd(data);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
