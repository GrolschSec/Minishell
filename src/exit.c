/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:33:03 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/16 21:48:50 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Frees a null-terminated array of strings.
 *
 * This function iterates through each string in the array, freeing each 
 * one, then frees the array itself. Assumes that the array and its elements
 * were dynamically allocated and are safe to free. If the array is NULL or 
 * contains non-dynamically-allocated elements, the behavior is undefined.
 *
 * param tab: Null-terminated array of strings to be freed.
 */
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

void	close_fds(t_data *data)
{
	int		x;

	x = 0;
	while (x < data->pipes)
	{
		if (data->exec[x].fdin != 0)
			close(data->exec[x].fdin);
		if (data->exec[x].fdout != 1)
			close(data->exec[x].fdout);
		x++;
	}
}

void	free_token_list(t_data *data)
{
	t_list *tmp;

	tmp = data->token_list;
	while (tmp != NULL)
	{
		if (tmp->var_env && tmp->var_env->nb_value != 0)
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
				ft_lstclear(&data->exec[i].outfile, del);
			if (data->exec[i].eof && data->exec[i].eof != NULL)
				ft_lstclear(&data->exec[i].eof, del);
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

/**
 * Exits the program and performs necessary cleanup.
 *
 * This function outputs an error message (if provided), frees all 
 * environment data and path information in the given data structure, 
 * then exits the program. Depending on the error code, it may also 
 * clear command data and exit with a failure status. It's assumed 
 * that the data structure and error message string (if not NULL) 
 * are properly initialized and safe to free or print. If not, 
 * the behavior is undefined.
 *
 * Args:
 *   data: Pointer to the data structure containing the environment 
 *         and path data to be freed, and potentially command data to 
 *         be cleared.
 *   err: Error code. If 1, command data is also cleared and the program 
 *        exits with a failure status.
 *   str: Error message string to be printed to stderr. If NULL, no 
 *        message is printed.
 */
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
