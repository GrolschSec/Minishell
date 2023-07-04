/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:32:12 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/03 16:34:09 by mrabourd         ###   ########.fr       */
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
		if (data->exec[x].cmd[0] == NULL)
			data->error = 2;
		x++;
	}
}

void	input_is_only_space(t_data *data, char *input)
{
	int	i;

	i = 0;
	while (input[i] && is_space(input[i]) == 1)
	{
		i++;
	}
	if ((int)ft_strlen(input) == i
		|| (input[i] == '!' && (int)ft_strlen(input) == 1)
		|| (input[i] == ':' && (int)ft_strlen(input) == 1))
	{
		*data->exit_code = 127;
		data->error = 1;
	}
}

void	parse_cmd(t_data *data)
{
	input_is_only_space(data, data->input);
	if (data->error == 0)
		data->str = ft_strtrim(data->input, " ");
	if (data->error == 0)
	{
		split_in_list(data);
		free (data->str);
	}
	if (data->error == 0)
		assign_type(data);
	// print_all(data);
	if (data->error == 0)
		count_pipes(data);
	if (data->error == 0)
		put_cmd_in_tab(data, data->pipes);
	if (data->error == 0)
		fill_files(data);
	if (data->error == 0)
		fill_eof(data);
	if (data->error == 0)
		open_files(data);
	if (data->error == 2)
		printf("minishell: syntax error near unexpected token `|'\n");
	// print_tab(data);
}
