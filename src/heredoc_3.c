/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 21:53:41 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/18 22:42:20 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	c_open_close_hd(t_list *tmp, t_data *data, int i)
{
	int	fd;

	if (!tmp->next && data->exec[i].is_eof)
		data->exec[i].fdin = heredoc(data, tmp->content, tmp->type, i);
	else
	{
		fd = heredoc(data, tmp->content, tmp->type, i);
		close(fd);
	}
}

char	*create_file(int *fd, int index)
{
	char	*path;

	path = get_hd_path(index);
	if (!path)
		return (NULL);
	*fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (*fd < 0)
	{
		free(path);
		return (NULL);
	}
	return (path);
}

int	handle_child_process(t_data *data, int fd, char *end, int type)
{
	signal(SIGINT, SIG_DFL);
	while (1)
		get_heredoc_in(data, fd, end, type);
	return (0);
}

int	process_fork(t_data *data, int fd, char *end, int type)
{
	int	status;
	int	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	signal(SIGINT, ft_signal_newline2);
	if (pid == 0)
	{
		free(data->tmp_path_hd);
		return (handle_child_process(data, fd, end, type));
	}
	waitpid(pid, &status, 0);
	return (status);
}

int	heredoc(t_data *data, char *end, int type, int index)
{
	int		fd;
	int		status;

	data->tmp_path_hd = create_file(&fd, index);
	if (!data->tmp_path_hd)
		return (-1);
	status = process_fork(data, fd, end, type);
	if (WIFEXITED(status))
	{
		close(fd);
		fd = open(data->tmp_path_hd, O_RDWR);
		return (free(data->tmp_path_hd), fd);
	}
	else if (status == 2)
		data->error = 3;
	return (close(fd), free(data->tmp_path_hd), -1);
}
