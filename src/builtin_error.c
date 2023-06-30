/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 18:43:34 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/06/29 18:44:40 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	invalid_option(char c)
{
	char	*tmp;
	char	*msg;

	if (c)
	tmp = ft_strjoin2("minishell: export: `-", &c);
	if (!tmp)
		return ;
	msg = ft_strjoin2(tmp, "': invalid option\n");
	free(tmp);
	if (!msg)
		return ;
	write(2, msg, ft_strlen(msg));
	free(msg);
	print_usage();
}

void	not_valid_identifier(char *arg)
{
	char	*tmp;
	char	*msg;

	tmp = ft_strjoin2("minishell: export: `", arg);
	if (!tmp)
		return ;
	msg = ft_strjoin2(tmp, "': not a valid identifier\n");
	free(tmp);
	if (!msg)
		return ;
	write(2, msg, ft_strlen(msg));
	free(msg);
}

void	print_usage(void)
{
	printf("export: usage: export [name[=value] ...] or export)\n");
}

void	export_no_arg(t_data *data)
{
	t_list	*tmp;

	tmp = data->env;
	while (tmp)
	{
		printf("declare -x %s\n", tmp->content);
		tmp = tmp->next;
	}
	return ;
}
