/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 18:07:16 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/27 16:08:58 by rlouvrie         ###   ########.fr       */
=======
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 18:07:16 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/25 18:01:06 by mrabourd         ###   ########.fr       */
>>>>>>> mrabourd
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Function: builtin_pwd
** ---------------------
** Built-in function to print the current working directory.
**
** Side effects:
** - Prints the current working directory to the standard output.
** - Frees memory allocated for the directory path string.
*/
void	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putstr_fd(pwd, 1);
	ft_putchar_fd('\n', 1);
	free(pwd);
}

/*
** Function: ft_handler
** --------------------
** Signal handler for interrupt signals (Ctrl+C).
**
** Args:
** - sig: Signal number.
**
** Side effects:
** - When an interrupt signal is received, a new line is printed 
**		and the input line is reset.
*/
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


/* 
	path est une variable d'env donc parse path n'est pas 
	a jour si on modifie la variable.
*/
int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		exit;

	(void)argv;
	(void)argc;
	exit = 0;
	if (!isatty(STDIN_FILENO))
		return (0);
	ft_bzero(&data, sizeof(data));
	data.exit_code = &exit;
	fill_env_list(env, &data);
	parse_path(&data);
	while (1)
	{
		signal(SIGINT, ft_handler);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
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
			execution(&data);
		}
	}
	return (0);
}
