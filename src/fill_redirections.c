/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 16:09:31 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/30 15:50:54 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Function: put_infiles_in_tab
** -----------------------------
** Allocates an array of strings to store input file names associated with a 
** command in a 't_exec' structure. Fills the array from a token list. Array 
** size depends on the number of input redirections in the command.
**
** Args:
** - data: Pointer to the main data structure.
** - tmp: Pointer to a token list.
** - current: Pointer to the 't_exec' structure to be filled.
**
** Side effects:
** In case of an error (e.g., allocation failure), the program exits with an
** error message. On success, input file names are copied into 'infile' array 
** in 'current'.
*/

void	put_infiles_in_tab(t_data *data, t_list *tmp, t_exec *current)
{
	int	nb_infile;

	nb_infile = 0;
	if (current->redirect_input <= 0)
		return ;
	else
		current->infile = malloc(sizeof(char *)
				* (current->redirect_input + 1));
	if (current->infile == NULL)
		exit_all(data, 1, "problem in redirect input");
	while (tmp != NULL && nb_infile < current->redirect_input)
	{
		if (tmp->type == INFILE)
		{
			current->infile[nb_infile] = ft_strdup(tmp->content);
			nb_infile++;
		}
		tmp = tmp->next;
	}
	current->infile[current->redirect_input] = NULL;
}

/*
** Function: put_outfiles_in_list
** -----------------------------
** Allocates an array of strings to store output file names linked with a 
** command in a 't_exec' structure. Fills the array from a token list. Array 
** size depends on the number of output redirections in the command.
**
** Args:
** - data: Pointer to the main data structure.
** - tmp: Pointer to a token list.
** - current: Pointer to the 't_exec' structure to be filled.
**
** Side effects:
** In case of error (e.g., allocation failure), the program exits with an
** error message. On success, output file names are copied into 'outfile' array 
** in 'current'.
*/
void	put_outfiles_in_list(t_data *data, t_list **tmp, t_exec *current)
{
	int		nb_outfile;
	t_list	*new;
	int		ret;

	ret = 0;
	new = NULL;
	nb_outfile = 0;
	if (current->redirect_output <= 0)
		return ;
	while ((*tmp) != NULL && nb_outfile < (current->redirect_output * 2))
	{
		if ((*tmp)->type == REDIRECT_OUTPUT || (*tmp)->type == DELIMITER_APPEND
			|| (*tmp)->type == OUTFILE)
		{
			new = ft_lstnew((*tmp)->content);
			ret = ft_lstadd_back(&current->outfile, new);
			if (ret == 1)
				exit_all(data, 1, "Problem when add_back in outfile list");
			nb_outfile++;
		}
		(*tmp) = (*tmp)->next;
	}
}

/*
** Function: fill_files
** ---------------------
** For each 't_exec' command in 'data', retrieves input and output files 
** from the token list. Stores them in 'infile' and 'outfile' arrays in the 
** corresponding 't_exec' structure.
**
** Args:
** - data: Pointer to the main data structure.
**
** Side effects:
** In case of an error, the program exits with an error message. On success,
** the 'infile' and 'outfile' arrays in each 't_exec' structure in 'data'
** are filled with corresponding input and output file names.
*/
void	fill_files(t_data *data)
{
	t_list	*tmp;
	int		x;

	x = 0;
	tmp = data->token_list;
	while (x < data->pipes)
	{
		put_infiles_in_tab(data, tmp, &data->exec[x]);
		if (tmp->type == PIPE && tmp != NULL)
			tmp = tmp->next;
		x++;
	}
	x = 0;
	tmp = data->token_list;
	while (x < data->pipes)
	{
		put_outfiles_in_list(data, &tmp, &data->exec[x]);
		if (tmp && (tmp->type == PIPE && tmp != NULL))
			tmp = tmp->next;
		x++;
	}
}
