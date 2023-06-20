/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 16:09:31 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 17:00:43 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	put_outfiles_in_tab(t_data *data, t_list *tmp, t_exec *current)
{
	int	nb_outfile;

	nb_outfile = 0;
	if (current->redirect_output <= 0)
		return ;
	else
		current->outfile = malloc(sizeof(char *) * (current->redirect_output + 1));
	if (current->outfile == NULL)
		exit_all(data, 1, "problem in redirect output");
	while (nb_outfile < current->redirect_output && tmp != NULL)
	{
		if (tmp->type == OUTFILE)
		{
			current->outfile[nb_outfile] = ft_strdup(tmp->content);
			nb_outfile++;
		}
		tmp = tmp->next;
	}
	current->outfile[current->redirect_output] = NULL;
}

void	put_infiles_in_tab(t_data *data, t_list *tmp, t_exec *current)
{
	int	nb_infile;

	nb_infile = 0;
	if (current->redirect_input <= 0)
		return ;
	else
		current->infile = malloc(sizeof(char *) * (current->redirect_input + 1));
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
		put_outfiles_in_tab(data, tmp, &data->exec[x]);
		if (tmp->type == PIPE && tmp != NULL)
			tmp = tmp->next;
		x++;
	}
}
