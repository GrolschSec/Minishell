/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 18:07:16 by mrabourd          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/07/19 20:48:10 by rlouvrie         ###   ########.fr       */
=======
/*   Updated: 2023/07/19 20:13:47 by mrabourd         ###   ########.fr       */
>>>>>>> origin/main
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
		num = 1;
	else
	{
		num = ft_atoi(value);
		free(value);
		num += 1;
	}
	new_val = ft_itoa(num);
	if (!new_val)
		return ;
	ft_setenv(data, "SHLVL=", new_val);
}

t_data	init_main(char **env)
{
	t_data	data;

	ft_bzero(&data, sizeof(data));
	fill_env_list(env, &data);
	update_shlvl(&data);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	ft_setenv(&data, "PWD=", getcwd(NULL, 0));
	return (data);
}

void	parse_and_exec(t_data *data)
{
	data->error = 0;
	add_history(data->input);
	parse_cmd(data);
	if (data->error == 0)
		execution(data);
	else
		clear_cmd(data);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argc;
	(void)argv;
	// if (!isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO))
	// 	return (0);
	data = init_main(env);
	while (1)
	{
		signal(SIGINT, ft_signal_newline);
		data.input = readline(COLOR_RED "Minishell> " COLOR_RESET);
		if (!data.input)
		{
			printf("exit\n");
			exit_all(&data, 0, NULL);
		}
		else if (data.input && data.input[0])
			parse_and_exec(&data);
	}
	return (0);
}
