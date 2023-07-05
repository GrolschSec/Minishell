/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 18:29:08 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/05 15:44:12 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
Comportement de exit:
	- Si exit est dans un pipe (ne marche pas)
	- Si exit sans arguments:
		- ecris exit
		- exit status code status de la derniere commande
	- Si exit un arguments numeric
		- ecris exit
		- exit status code donner en arguments
	- Si nombres d'arguments plus que 1 (ne marches pas)
		- ecris exit
		- ecris message "bash: exit: too many arguments"
		- newline on bash
		- status code 1
	- Si argument de exit pas numeric: 
		- ecris exit 
		- ecris message "bash: exit: dm,dkmd: numeric argument required"
		- analyse du message erreur: 
			- "minishell: exit: `premier arguments donner`: numeric argument required"
		- exit code erreur 2
	Cas non gerers actuellement:
		Cas a gerer directement dans le parsing (tester avec expr):
			- $> exit '6'66
			- $> exit '2'66'32'
			- $> exit '666'"666"666
			- $> exit +'666'"666"666
			- $> exit -'666'"666"666
*/

void	exit_builtin(t_data *data, t_exec *exec)
{
	int		ex;

	ex = 1;
	if (!(data->pipes > 1))
		printf("exit\n");
	if (exec->nb_cmd > 1)
	{
		if (!str_isdigit(exec->cmd[1]) || is_out_of_range(exec->cmd[1]))
		{
			numeric_arg_error(exec);
			g_exit = 2;
		}
		else if (exec->nb_cmd > 2)
		{
			exec_error("exit", "too many arguments");
			g_exit = 1;
			ex = 0;
		}
		else
			g_exit = convert_to_exit_code(ft_atoi(exec->cmd[1]));
	}
	perform_exit(ex, data, exec);
}

int	str_isdigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	convert_to_exit_code(long long nb)
{
	int	code;

	if (nb >= 0 && nb <= 255)
		return (nb);
	if (nb < 0)
		code = 256 - (-nb % 256);
	else
		code = nb % 256;
	return (code);
}

void	numeric_arg_error(t_exec *exec)
{
	char	*msg;

	msg = ft_strjoin2(exec->cmd[1], ": numeric argument required");
	if (!msg)
		return ;
	exec_error("exit", msg);
	free(msg);
}

void	perform_exit(int ex, t_data *data, t_exec *exec)
{
	if (ex)
	{
		if (data->pipes == 1)
		{
			close(data->cpy_in);
			close(data->cpy_out);
			exit_minishell(data);
		}
		else if (data->pipes > 1 && !exec->is_last)
		{
			close(data->cpy_in);
			close(data->cpy_out);
			exit_ps(data, g_exit);
		}
		else if (data->pipes > 1 && exec->is_last)
			return ;
	}
	else
		return ;
}
