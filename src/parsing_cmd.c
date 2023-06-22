/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:32:12 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/22 18:48:44 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_tab(t_data *data) /* print pour checker si c'est bon (a virer plus tard) */
{
	int	i;
	int	nb_jobs;
	int	infile;
	int	outfile;
	int nb_eof;
	
	nb_eof = 0;
	outfile = 0;
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
			printf("nb d'infiles: %d\n", data->exec[nb_jobs].redirect_input);
			while (infile < data->exec[nb_jobs].redirect_input)
			{
				printf("infile[%d]: %s\n", infile, data->exec[nb_jobs].infile[infile]);
				infile++;
			}
		}
		if (data->exec[nb_jobs].redirect_output > 0)
		{
			outfile = 0;
			while (outfile < data->exec[nb_jobs].redirect_output)
			{
				printf("outfile[%d]: %s\n", outfile, data->exec[nb_jobs].outfile[outfile]);
				outfile++;
			}
		}
		if (data->exec[nb_jobs].heredoc > 0)
		{
			nb_eof = 0;
			while (nb_eof < data->exec[nb_jobs].heredoc)
			{
				printf("eof[%d]: %s\n", nb_eof, data->exec[nb_jobs].eof[nb_eof]);
				nb_eof++;
			}
		}
		nb_jobs++;
	}
}

int	is_redirection(t_list *tmp) /* check si n'est pas une commande mais une redirection */
{
	if (tmp->type == INFILE || tmp->type == OUTFILE
		|| tmp->type == REDIRECT_INPUT || tmp->type == REDIRECT_OUTPUT
		|| tmp->type == HEREDOC || tmp->type == ENDOFFILE
		|| tmp->type == DELIMITER_APPEND)
		return (1);
	return (0);
}

void	fill_exec(t_data *data, t_list **tmp, t_exec *current, int x) /* remplit les exec_cmd pour chaque exec */
{
	int		y;
	t_list	*count;

	y = 0;
	count = *tmp;
	current[x].nb_cmd = count_cmd(count);
	init_exec(current, x);
	current[x].cmd = malloc(sizeof(char *) * (current[x].nb_cmd + 1));
	if (current[x].cmd == NULL)
		exit_all(data, 1, "malloc probleme pour structure");
	while (y < current[x].nb_cmd)
	{
		if ((*tmp)->next != NULL && is_redirection(*tmp) == 1)
			count_redirections(*tmp, current, x);
		else
			current[x].cmd[y++] = ft_strdup((*tmp)->content);
		*tmp = (*tmp)->next;
	}
	while ((*tmp) && ((*tmp)->type == PIPE || is_redirection(*tmp)))
	{
		count_redirections(*tmp, current, x);
		*tmp = (*tmp)->next;
	}
	current[x].cmd[y] = NULL;
}

void	put_cmd_in_tab(t_data *data, int nb) /* malloc le nombre de t_exec en fonction du nb de pipes */
{
	t_list	*tmp;
	int		x;

	x = 0;
	tmp = data->token_list;
	data->exec = malloc(sizeof(t_exec) * nb);
	if (data->exec == NULL)
		exit_all(data, 1, "malloc probleme pour structure");
	while (x < nb)
	{
		fill_exec(data, &tmp, data->exec, x);
		x++;
	}
}

void	parse_cmd(t_data *data)
{
	char	*str;
	int		i;

	i = 0;
	str = NULL;
	str = ft_strtrim(data->input, " ");
	split_in_list(data, str);
	free (str);
	assign_type(data);
	// print_all(data);
	count_pipes(data);
	put_cmd_in_tab(data, data->pipes);
	fill_files(data);
	fill_eof(data);
	open_files(data);
	print_tab(data);
}
