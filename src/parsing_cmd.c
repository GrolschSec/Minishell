/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:32:12 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/12 17:27:14 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int is_redirection(t_list *tmp)
{
	if (tmp->type == INFILE || tmp->type == OUTFILE || tmp->type == REDIRECT_INPUT || tmp->type == REDIRECT_OUTPUT || tmp->type == HEREDOC || tmp->type == ENDOFFILE || tmp->type == DELIMITER_APPEND)
		return (1);
	return (0);
}

void fill_exec(t_data *data, t_list **tmp, t_exec *current, int x)
{
	int y;

	y = 0;
	current[x].cmd = malloc(sizeof(char *) * (current[x].nb_cmd + 1));
	if (current[x].cmd == NULL)
		exit_all(data, 1, "malloc probleme pour structure");
	while (*tmp != NULL && y < current[x].nb_cmd)
	{
		if ((*tmp) != NULL && is_redirection(*tmp) == 0)
			current[x].cmd[y++] = ft_strdup((*tmp)->content);
		*tmp = (*tmp)->next;
	}
	while ((*tmp) && ((*tmp)->type == PIPE || is_redirection(*tmp)))
	{
		*tmp = (*tmp)->next;
	}
	current[x].cmd[y] = NULL;
}

void put_cmd_in_tab(t_data *data, int nb)
{
	t_list *tmp;
	int x;

	x = 0;
	tmp = data->token_list;
	while (x < nb)
	{
		fill_exec(data, &tmp, data->exec, x);
		x++;
	}
}

void input_is_only_space(t_data *data, char *input)
{
	int i;

	i = 0;
	while (input[i] && is_space(input[i]) == 1)
	{
		i++;
	}
	if ((int)ft_strlen(input) == i || (input[i] == '!' && (int)ft_strlen(input) == 1) || (input[i] == ':' && (int)ft_strlen(input) == 1))
	{
		g_exit = 127;
		data->error = 1;
	}
}

void	check_if_nothing(t_data *data)
{
	int	x;

	x = 0;
	while (x < data->pipes)
	{
		if (data->exec[x].nb_cmd == 0
				&& data->exec[x].redirect_input == 0 
				&& data->exec[x].redirect_output == 0 
				&& data->exec[x].heredoc == 0)
		{
			data->error = 2;
			g_exit = 2;
			return ;
		}
		x++;
	}
}

void parse_cmd(t_data *data)
{
	input_is_only_space(data, data->input);
	if (data->error == 0)
		data->str = ft_strtrim(data->input, " ");
	if (data->error == 0)
	{
		split_in_list(data);
		free(data->str);
	}
	if (data->error == 0)
		assign_type(data);
	// if (data->error == 0)
	count_pipes(data);
	print_all(data);
	if (data->error == 0)
		fill_eof(data, data->pipes);
	check_if_nothing(data);
	heredoc_check(data);
	if (data->error == 0)
		put_cmd_in_tab(data, data->pipes);
	if (data->error == 0)
		fill_files(data);
	if (data->error == 0)
		open_files(data);
	if (data->error == 2)
		printf("minishell: syntax error near unexpected token `|'\n");
	// print_tab(data);
}
