/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:32:12 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/29 16:52:25 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_redirection(t_list *tmp)
{
	if (tmp->type == INFILE || tmp->type == OUTFILE
		|| tmp->type == REDIRECT_INPUT || tmp->type == REDIRECT_OUTPUT
		|| tmp->type == HEREDOC || tmp->type == ENDOFFILE
		|| tmp->type == DELIMITER_APPEND)
		return (1);
	return (0);
}

void	fill_exec(t_data *data, t_list **tmp, t_exec *current, int x)
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
		{
			count_redirections(*tmp, current, x);
		}
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

void	put_cmd_in_tab(t_data *data, int nb)
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
	data->str = ft_strtrim(data->input, " ");
	split_in_list(data);
	free (data->str);
	assign_type(data);
	// print_all(data);
	count_pipes(data);
	put_cmd_in_tab(data, data->pipes);
	fill_files(data);
	fill_eof(data);
	open_files(data);
	print_tab(data);
}
