/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 15:31:52 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/17 15:48:44 by rlouvrie         ###   ########.fr       */
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
		close(fd);
		free(input);
		free(end);
		exit_heredoc(data, 0);
	}
	if (ft_strncmp(input, end, ft_strlen(end)) == 0
		&& ft_strlen(end) == ft_strlen(input))
	{
		close(fd);
		free(input);
		free(end);
		exit_heredoc(data, 0);
	}
	tmp = convert_input(input, data, type);
	free(input);
	if (tmp)
		ft_putstr_fd(tmp, fd);
	ft_putchar_fd('\n', fd);
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

int	heredoc(t_data *data, char *end, int type, int index)
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
		close(fd);
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
}

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
	int		fd;

	i = 0;
	while (i < data->pipes)
	{
		tmp = data->exec[i].eof;
		if (data->exec[i].heredoc > 0)
		{
			while (tmp)
			{
				if (!tmp->next && data->exec[i].is_eof)
					data->exec[i].fdin = heredoc(data, tmp->content, tmp->type, i);
				else
				{
					fd = heredoc(data, tmp->content, tmp->type, i);
					close(fd);
				}
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

void	exit_heredoc(t_data *data, int error)
{
	int	i;

	i = 0;
	if (data->token_list && data->token_list != NULL)
		ft_lstclear(&data->token_list, del);
	if (data->pipes > 0)
	{
		while (i < data->pipes)
		{
			if (data->exec[i].cmd && data->exec[i].cmd != NULL)
				free_tab(data->exec[i].cmd);
			if (data->exec[i].infile && data->exec[i].infile != NULL)
				free_tab(data->exec[i].infile);
			if (data->exec[i].outfile && data->exec[i].outfile != NULL)
				ft_lstclear(&data->exec[i].outfile, del);
			i++;
		}
		if (data->exec && data->exec != NULL)
			free(data->exec);
	}
	free_env(data);
	if (data->tilde)
		free(data->tilde);
	if (data->path.tab)
		free_tab(data->path.tab);
	if (data->path.line)
		free(data->path.line);
	exit(error);
}
