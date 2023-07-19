/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 16:48:42 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/19 21:05:37 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*split_get_value(char *str)
{
	int		i;
	int		j;
	char	*ret;

	ret = NULL;
	j = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	if ((int)ft_strlen(str) == i)
		return (NULL);
	i++;
	j = i;
	while (str[i])
		i++;
	ret = ft_substr(str, j, i - j);
	return (ret);
}

char	*get_value(t_list *env)
{
	char	*split;

	split = split_get_value(env->content);
	if (!split)
		return (NULL);
	return (split);
}

char	*ft_getenv(t_data *data, char *name)
{
	t_list	*tmp;
	char	*str_tmp;

	str_tmp = ft_strjoin2(name, "=");
	if (!str_tmp)
		return (NULL);
	tmp = data->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, str_tmp, ft_strlen(str_tmp)) == 0)
			break ;
		tmp = tmp->next;
	}
	free(str_tmp);
	str_tmp = NULL;
	if (tmp)
	{
		str_tmp = get_value(tmp);
		if (!str_tmp)
			return (NULL);
		return (str_tmp);
	}
	return (NULL);
}

void	else_cond_ft_setenv(t_list *env, char *new_env)
{
	char	*tmp_str;

	tmp_str = env->content;
	env->content = new_env;
	free(tmp_str);
}

void	parent_no_exist(t_data *data, t_exec *exec)
{
	(void)data;
	(void)exec;
	ft_putstr_fd("chdir: error retrieving current directory: ", 2);
	perror("get_cwd: cannot access parent directories");
}
