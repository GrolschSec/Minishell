/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 18:07:16 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 15:38:43 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_all(t_data *data)/* A supprimer aussi */
{
	t_list	*tmp;

	tmp = data->token_list;
	printf("----START ALL-----\n");
	while (tmp != NULL)
	{
		printf("%s\n", tmp->content);
		printf("type: %u\n", tmp->type);
		// printf("%d\n", tmp->printed);
		tmp = tmp->next;
	}
	// printf("%d\n", tmp->printed);
	printf("----END ALL-----\n");
}

void	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putstr_fd(pwd, 1);
	ft_putchar_fd('\n', 1);
	free(pwd);
}

void	ft_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	exec_cmd(t_data *data)
{
	int	i;
	int	y;

	y = 0;
	i = 0;
	while (i < data->pipes)
	{
		y = 0;
		while (y < data->exec[i].nb_cmd)
		{
			if (ft_strncmp(data->exec[i].cmd[y], "export", ft_strlen(data->exec[i].cmd[y])) == 0)
				builtin_export(data, &data->exec[i].cmd[y]);
			if (ft_strncmp(data->exec[i].cmd[y], "env", ft_strlen(data->exec[i].cmd[y])) == 0)
				print_env_tab(data);
			if (ft_strncmp(data->exec[i].cmd[y], "echo", ft_strlen(data->exec[i].cmd[y])) == 0)
				builtin_echo_str(data, &data->exec[i].cmd[y]);
			if (ft_strncmp(data->exec[i].cmd[y], "pwd", ft_strlen(data->exec[i].cmd[y])) == 0)
				builtin_pwd();
			// if (ft_strncmp(data->exec[i].cmd[y], "unset", ft_strlen(data->exec[i].cmd[y])) == 0)
			// 	builtin_unset(data, data->exec[i].cmd[y]); /* a retoucher */
			y++;
		}
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	(void)argv;

	ft_bzero(&data, sizeof(data));
	if (!env || env == NULL || argc != 1)
		exit_all(&data, 1, "There is a problem with the arguments or the environment");
	fill_env_list(env, &data);
	parse_path(&data);
	// print_env_tab(&data); -> pour tester si env ok
	while (1)
	{
		signal(SIGINT, ft_handler);
		signal(SIGQUIT, SIG_IGN);
		data.input = readline("Minishell>");
		if (!data.input)
		{
			exit_all(&data, 0, NULL);
		}
		add_history(data.input);
		parse_cmd(&data);
		exec_cmd(&data); /* mini fonction exec pour tester certains builtins */
		// execution(&data); --> a ajouter
		clear_cmd(&data);
	}
	return (0);
}
