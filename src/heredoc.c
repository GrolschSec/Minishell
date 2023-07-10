/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 15:31:52 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/10 18:07:39 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_heredoc_in(t_data *data, int fd, char *end)
{
	char	*input;

	input = readline("> ");
	if (!input)
	{
		write(2, "minishell : warning : heredoc delimited by end-of-file\n", 55);
		close(fd);
		free(input);
		free(end);
		exit_ps(data, 0);
	}
	if (ft_strncmp(input, end, ft_strlen(end)) == 0)
	{
		close(fd);
		free(input);
		free(end);
		exit(0);
	}
	// Convert to input.
	ft_putstr_fd(input, fd);
	ft_putchar_fd('\n', fd);
	free(input);
}

int	heredoc(t_data *data, char *end)
{
	int		fd;
	int		pid;
	int		status;

	fd = open("/tmp/.h", O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (fd < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (close(fd), -1);
	if (pid == 0)
	{
		while (1)
			get_heredoc_in(data, fd, end);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		close(fd);
		return (open("/tmp/.h", O_RDWR));
	}
	return (close(fd), -1);
}

void	heredoc_check(t_data *data)
{
	int	i;
	int	j;
	int	old_fd;

	i = 0;
	while (i < data->pipes)
	{
		j = 0;
		if (data->exec[i].heredoc > 0)
		{
			while (j < data->exec[i].heredoc)
			{
				old_fd = data->exec[i].fdin;
				data->exec[i].fdin = heredoc(data, data->exec[i].eof[j]);
				close(old_fd);
				j++;
			}
		}
		i++;
	}
}
