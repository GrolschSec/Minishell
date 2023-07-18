/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 15:31:52 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/18 22:25:14 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_heredoc_in(t_data *data, int fd, char *end, int type)
{
	char	*input;
	char	*tmp;

	input = readline("> ");
	if (!input)
	{
		print_heredoc_error(end);
		exit_heredoc(data, fd, input);
	}
	if (ft_strncmp(input, end, ft_strlen(end)) == 0
		&& ft_strlen(end) == ft_strlen(input))
	{
		exit_heredoc(data, fd, input);
	}
	tmp = convert_input(input, data, type);
	free(input);
	if (tmp)
	{
		ft_putstr_fd(tmp, fd);
		free(tmp);
	}
	ft_putchar_fd('\n', fd);
}

/*int	heredoc(t_data *data, char *end, int type, int index)
{
	int		fd;
	int		pid;
	int		status;
	char	*path;

	path = get_hd_path(index);
	if (!path)
		return (-1);
	fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (fd < 0)
		return (free(path), -1);
	pid = fork();
	if (pid < 0)
		return (close(fd), free(path), -1);
	signal(SIGINT, ft_signal_newline2);
	if (pid == 0)
	{
		free(path);
		signal(SIGINT, SIG_DFL);
		while (1)
			get_heredoc_in(data, fd, end, type);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		close(fd);
		fd = open(path, O_RDWR);
		return (free(path), fd);
	}
	else if (status == 2)
		data->error = 3;
	return (close(fd), free(path), -1);
}*/

void	add_char_to_str(char **input, char c)
{
	char	*c_input;

	if (!*(input))
	{
		c_input = malloc(sizeof(char) * 2);
		if (!c_input)
			return ;
		c_input[0] = c;
		c_input[1] = '\0';
	}
	else
	{
		c_input = malloc(sizeof(char) * (ft_strlen(*input) + 2));
		if (!c_input)
			return ;
		ft_strlcpy(c_input, *input, ft_strlen(*input) + 1);
		c_input[ft_strlen(*input)] = c;
		c_input[ft_strlen(*input) + 1] = '\0';
		free(*input);
	}
	*input = c_input;
}

void	heredoc_check(t_data *data)
{
	int		i;
	t_list	*tmp;

	i = 0;
	while (i < data->pipes)
	{
		tmp = data->exec[i].eof;
		if (data->exec[i].heredoc > 0)
		{
			while (tmp)
			{
				c_open_close_hd(tmp, data, i);
				tmp = tmp->next;
			}
		}
		i++;
	}
}

char	*convert_input(char *input, t_data *data, int type)
{
	int		i;
	char	*c_input;

	i = 0;
	c_input = NULL;
	while (input[i])
	{
		if (type != ENDOFFILE_QUOTED && input[i] == '$')
			i = handle_env_var(i, &input[i], &c_input, data);
		else
			add_char_to_str(&c_input, input[i]);
		i++;
	}
	return (c_input);
}
