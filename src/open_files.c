/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 19:24:51 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/15 16:43:32 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	open_outfile(t_data *data, int x, t_list *out)
{
	while (out != NULL)
	{
		if (data->exec[x].fdout && data->exec[x].fdout != 1)
			close(data->exec[x].fdout);
		if (ft_strncmp(out->content, ">", 2) == 0)
		{
			out = out->next;
			data->exec[x].fdout = open(out->content,
					O_CREAT | O_RDWR | O_TRUNC, 0644);
		}
		else if (ft_strncmp(out->content, ">>", 2) == 0)
		{
			out = out->next;
			data->exec[x].fdout = open(out->content,
					O_CREAT | O_RDWR | O_APPEND, 0644);
		}
		if (data->exec[x].fdout == -1)
			printf("Fail to open fdout\n");
		out = out->next;
	}
	if (data->exec[x].redirect_output && data->exec[x].nb_cmd == 0)
	{
		data->error = 1;
		g_exit = 1;
	}
}

void	open_infile(t_data *data, int x, int in)
{
	in = data->exec[x].redirect_input;
	if (data->exec[x].fdin != 0 && !data->exec[x].is_eof)
		close(data->exec[x].fdin);
	if (access(data->exec[x].infile[in - 1], R_OK) != 0)
	{
		exec_error(data->exec[x].infile[in - 1], "No such file or directory");
		data->error = 1;
		g_exit = 1;
	}
	else if (data->exec[x].cmd[0] != NULL && !data->exec[x].is_eof)
	{
		data->exec[x].fdin = open(data->exec[x].infile[in - 1],
				O_RDONLY);
		if (data->exec[x].fdin < 0)
			printf("Fail to open fdin\n");
	}
	else if (data->exec[x].is_eof)
		return ;
	else
	{
		data->error = 1;
		g_exit = 1;
	}
}

void	open_files(t_data *data)
{
	int		x;
	int		in;
	t_list	*out;

	out = NULL;
	in = 0;
	x = 0;
	while (x < data->pipes)
	{
		if (data->exec[x].redirect_input > 0)
			open_infile(data, x, in);
		out = data->exec[x].outfile;
		if (out != NULL)
			open_outfile(data, x, out);
		x++;
	}
}
