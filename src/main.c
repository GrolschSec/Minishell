/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 18:07:16 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/17 19:43:30 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_exit = 0;

void	update_shlvl(t_data *data)
{
	char	*value;
	int		num;
	char	*new_val;

	value = ft_getenv(data, "SHLVL");
	if (!value)
		return ;
	num = ft_atoi(value);
	free(value);
	num += 1;
	new_val = ft_itoa(num);
	if (!new_val)
		return ;
	ft_setenv(data, "SHLVL=", new_val);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argv;
	(void)argc;
	// if (!isatty(STDIN_FILENO))
	// 	return (0);
	ft_bzero(&data, sizeof(data));
	fill_env_list(env, &data);
	update_shlvl(&data);
	while (1)
	{
		data.error = 0;
		signal(SIGINT, ft_signal_newline);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		data.input = readline(COLOR_RED "Minishell> " COLOR_RESET);
		if (!data.input)
		{
			printf("exit\n");
			exit_all(&data, 0, NULL);
		}
		else if (data.input && data.input[0])
		{
			add_history(data.input);
			parse_cmd(&data);
			if (data.error == 0)
				execution(&data);
			else
				clear_cmd(&data);
		}
	}
	return (0);
}
