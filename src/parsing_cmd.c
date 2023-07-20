/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:32:12 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/20 13:09:42 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fill_var_env(t_list **tmp, t_exec current, int *y)
{
	int	z;

	z = 0;
	while (z < (*tmp)->var_env->nb_value)
	{
		current.cmd[*y] = ft_strdup((*tmp)->var_env->value[z]);
		(*y)++;
		z++;
	}
}

void	fill_exec(t_data *data, t_list **tmp, t_exec *current, int x)
{
	int	y;

	y = 0;
	current[x].cmd = malloc(sizeof(char *) * (current[x].nb_cmd + 1));
	if (current[x].cmd == NULL)
		exit_all(data, 1, "malloc probleme pour structure");
	while (*tmp != NULL && y < current[x].nb_cmd)
	{
		if ((*tmp) != NULL && is_redir(*tmp) == 0 && (*tmp)->var_env == NULL)
		{
			current[x].cmd[y] = ft_strdup((*tmp)->content);
			y++;
		}
		else if ((*tmp)->var_env && (*tmp)->var_env->nb_value != 0)
			fill_var_env(tmp, current[x], &y);
		*tmp = (*tmp)->next;
	}
	while ((*tmp) && ((*tmp)->type == PIPE || is_redir(*tmp)))
	{
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
	while (x < nb && data->error == 0)
	{
		fill_exec(data, &tmp, data->exec, x);
		if (data->exec[x].nb_cmd != 0
			&& data->exec[x].cmd != NULL
			&& ft_strncmp(&data->exec[x].cmd[0][0], "\0", 1) == 0)
		{
			data->exec[x].no_cmd = 1;
			g_exit = 0;
		}
		x++;
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
		free(data->str);
	}
	if (data->error == 0)
		assign_type(data);
	count_pipes_cmd_redir(data);
	if (data->error == 0)
		fill_eof(data, data->pipes);
	check_if_nothing(data);
	if (data->error == 0)
		heredoc_check(data);
	if (data->error == 0)
		put_cmd_in_tab(data, data->pipes);
	if (data->error == 0)
		fill_files(data);
	if (data->error == 0)
		open_files(data);
	if (data->error == 2)
		error (data, "'|'");
}
