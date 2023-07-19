/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 21:46:41 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/18 21:47:57 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_env_var(int i, char *input, char **c_input, t_data *data)
{
	int		j;
	int		len;
	char	*var;
	char	*value;

	j = 1;
	len = 0;
	if (!ft_isprint(input[j]))
		return (add_char_to_str(c_input, input[i]), i);
	else if (ft_isdigit(input[j]))
		return (i + j);
	else if (ft_isalpha(input[j]) || input[j] == '_')
	{
		while (input[j] && (ft_isalnum(input[j]) || input[j] == '_'))
			j++;
		len = j - 1;
		var = malloc(sizeof(char) * (len + 1));
		if (!var)
			return (i);
		ft_strlcpy(var, input + 1, len + 1);
		value = ft_getenv(data, var);
		if (value)
			add_var_to_input(c_input, value);
	}
	return (i + len);
}

void	add_var_to_input(char **c_input, char *value)
{
	char	*tmp;

	if (!*c_input)
	{
		*c_input = value;
		return ;
	}
	else
	{
		tmp = ft_strjoin2(*c_input, value);
		free(value);
		if (tmp)
		{
			free(*c_input);
			*c_input = tmp;
		}	
	}
}

void	print_heredoc_error(char *end)
{
	write(2, "minishell: warning: ", 20);
	write(2, "here-document delimited by end-of-file (wanted `", 48);
	if (end)
		write(2, end, ft_strlen(end));
	write(2, "')\n", 3);
}

void	exit_heredoc(t_data *data, int fd, char *input)
{
	close(fd);
	if (input)
		free(input);
	clear_cmd(data);
	free_env(data);
	free(data->tilde);
	exit(0);
}

char	*get_hd_path(int i)
{
	char	*path;
	char	*index;

	index = ft_itoa(i);
	if (!index)
		return (NULL);
	path = ft_strjoin2("/tmp/.h", index);
	if (!path)
		return (free(index), NULL);
	return (free(index), path);
}
