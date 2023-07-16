/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:04:18 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/16 11:12:46 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_signal_quit(int unused)
{
	(void)unused;
	write(2, "Quit (core dumped)\n", 19);
	g_exit = 131;
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
void	ft_signal_newline(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit = 130;
	}
}

void	ft_signal_newline2(int unused)
{
	(void)unused;
	write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_exit = 130;
}
