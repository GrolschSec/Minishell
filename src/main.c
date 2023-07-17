/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 18:07:16 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/17 19:06:22 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_exit = 0;

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argv;
	(void)argc;
	if (!isatty(STDIN_FILENO))
		return (0);
	ft_bzero(&data, sizeof(data));
	fill_env_list(env, &data);
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
