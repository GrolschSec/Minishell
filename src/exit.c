/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:33:03 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/21 18:35:47 by mrabourd         ###   ########.fr       */
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

/**
 * Clears the command data structure.
 *
 * This function cleans up memory allocated for tokens, command, input, 
 * and output files in the given data structure. It also resets certain 
 * data fields to their default values, such as file descriptors and 
 * redirection flags. The data structure fields must be correctly 
 * initialized before calling this function, else the behavior is undefined.
 *
 * Args:
 *   data: Pointer to the data structure to be cleared. Expected to contain 
 *         a token list, array of command structures (with fields for command, 
 *         input file, and output file), and an integer for the number of pipes.
 */
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
			if (data->exec[i].eof)
				free_tab(data->exec[i].eof);
			data->exec[i].fdin = 0;
			data->exec[i].fdout = 1;
			data->exec[i].redirect_input = 0;
			data->exec[i].redirect_output = 0;
			data->exec[i].heredoc = 0;
			data->exec[i].delimiter_append = 0;
			data->exec[i].nb_cmd = 0;
			i++;
		}
	}
}

/**
 * Frees the environment data in the provided data structure.
 *
 * This function clears the environment list and the environment table 
 * in the provided data structure, freeing any associated memory. It 
 * assumes the environment data is correctly initialized and safe to free. 
 * If not, the behavior is undefined.
 *
 * Args:
 *   data: Pointer to the data structure containing the environment 
 *         data to be freed. It is expected to contain a list of 
 *         environment variables and a table of environment strings.
 */
void	free_env(t_data *data)
{
	if (data->env && data->env != NULL)
		ft_lstclear(&data->env, del);
	if (data->env_tab && data->env_tab != NULL)
		free_tab(data->env_tab);
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
