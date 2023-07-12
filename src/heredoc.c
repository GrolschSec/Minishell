/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 15:31:52 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/12 19:46:03 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_heredoc_in(t_data *data, int fd, char *end)
{
	char	*input;

	input = readline("> ");
	if (!input)
	{
		print_heredoc_error(end);
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

	i = 0;
	while (i < data->pipes)
	{
		j = 0;
		if (data->exec[i].heredoc > 0)
		{
			while (j < data->exec[i].heredoc)
			{
				data->exec[i].fdin = heredoc(data, data->exec[i].eof[j]);
				j++;
			}
		}
		i++;
	}
}

char	*convert_input(char *input)
{
	int		i;
	char	*c_input;

	i = 0;
	c_input = NULL;
	while (input[i])
	{
		if (input[i] == '$')
			i = handle_env_var(i, &input[i], c_input);
		//else
		//	add_char_to_str();
		i++;
	}
	return (input);
}

int	handle_env_var(int	i, char *input, char *c_input)
{
	int		j;
	int		len;
	char	*var;

	(void)var;
	(void)c_input;
	j = 1;
	if (!ft_isprint(input[j]))
		return (/*add_char_to_str(c_input, input[i]), */i);
	else if (ft_isdigit(input[j]))
		return (i + j);
	else if (ft_isalpha(input[j]) || input[j] == '_')
	{
		len = 0;
		while(input[j] && (ft_isalnum(input[j]) || input[j] == '_'))
			j++;
		len = j - 1;
	}
	return (i + j);
}

void	print_heredoc_error(char *end)
{
	write(2, "minishell: warning: ", 20);
	write(2, "here-document delimited by end-of-file (wanted `", 48);
	if (end)
		write(2, end, ft_strlen(end));
	write(2, "')\n", 3);
}
