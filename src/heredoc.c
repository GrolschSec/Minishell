/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 15:31:52 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/12 14:05:26 by rlouvrie         ###   ########.fr       */
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
	convert_input(input, data);
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

char	*convert_input(char *input, t_data *data)
{
	int		i;
	char	*c_input;

	i = 0;
	c_input = NULL;
	while (input[i])
	{
		if (input[i] == '$')
			i = handle_env_var(i, &input[i], c_input, data);
		//else
		//	add_char_to_str();
		i++;
	}
	return (input);
}

int	handle_env_var(int	i, char *input, char *c_input, t_data *data)
{
	int		j;
	int		len;
	char	*var;
	char	*value;

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
		var = malloc(sizeof(char) * (len + 1));
		if (!var)
			return (i);
		ft_strlcpy(var, input + 1, len + 1);
		value = ft_getenv(data, var);
		if (value)
		{
			
			ft_strlcat()
		}
		return (i + j);
		
	}
	return (i + j);
}
