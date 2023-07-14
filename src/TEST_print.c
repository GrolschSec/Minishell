/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TEST_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 18:00:28 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/14 16:11:21 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Function: print_all
** -------------------
** Prints all tokens in the list for debugging. (Function will be removed)
**
** Args:
** - data: Pointer to the main data structure.
**
** Side effects:
** - Prints the content and type of each token in the list.
*/
void	print_all(t_data *data)
{
	t_list	*tmp;

	tmp = data->token_list;
	printf("----START ALL-----\n");
	while (tmp != NULL)
	{
		printf("list: %s\n", tmp->content);
		printf("type: %d\n", tmp->type);
		tmp = tmp->next;
	}
	printf("----END ALL-----\n");
}

void	print_tab(t_data *data)
{
	int		i;
	int		nb_jobs;
	int		infile;
	t_list	*tmp_out;
	t_list	*tmp_eof;
	int		nb_eof;

	nb_eof = 0;
	infile = 0;
	nb_jobs = 0;
	i = 0;
	while (nb_jobs < data->pipes)
	{
		i = 0;
		while (i < data->exec[nb_jobs].nb_cmd)
		{
			// printf("nb_cmd: %d\n", data->exec[nb_jobs].nb_cmd);
			printf("data->exec[%d].cmd[%d]: %s\n", nb_jobs, i, data->exec[nb_jobs].cmd[i]);
			i++;
		}
		if (data->exec[nb_jobs].redirect_input > 0)
		{
			infile = 0;
			while (infile < data->exec[nb_jobs].redirect_input)
			{
				printf("infile[%d]: %s\n", infile, data->exec[nb_jobs].infile[infile]);
				infile++;
			}
		}
		tmp_out = data->exec[nb_jobs].outfile;
		if (tmp_out != NULL)
		{
			while (tmp_out != NULL)
			{
				printf("outfile[%d]: %s\n", nb_jobs, tmp_out->content);
				tmp_out = tmp_out->next;
			}
		}
		tmp_eof= data->exec[nb_jobs].eof;
		if (data->exec[nb_jobs].heredoc > 0)
		{
			while (tmp_eof != NULL)
			{
				printf("eof[%d]: %s\n", nb_jobs, tmp_eof->content);
				tmp_eof = tmp_eof->next;
			}
		}
		nb_jobs++;
	}
}
