/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 13:29:02 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 18:21:14 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "../libft/libft.h"

typedef struct s_path
{
	char	*line;
	char	**tab;
}				t_path;

/*
** Structure for command execution
**
** cmd: Tab of commands between each pipe
** infile: Name of each infile between each pipe
** outfile: Name of each outfile between each pipe
** fdin: fdin, defaults to 0
** fdout: fdout, defaults to 1
** redirect_input: Number of '<' between each pipe
** redirect_output: Number of '>'
** heredoc: Number of '<<'
** delimiter_append: Number of '>>'
** nb_cmd: Number of commands in the cmd tab
*/
typedef struct s_exec
{
	char		**cmd;
	char		**infile;
	char		**outfile;
	int			fdin;
	int			fdout;
	int			redirect_input;
	int			redirect_output;
	int			heredoc;
	int			delimiter_append;
	int			nb_cmd;
}				t_exec;

/*
** s_data: Data structure for minishell.
** input: User input string.
** pipes: Number of pipes + 1.
** exec: Array of 't_exec' structures, sized per the number of pipes + 1.
** token_list: Input transformed into a linked list. See libft .h file.
** env: Linked list of environment variables.
** env_tab: Environment in character array format.
** nb_env: Variable count in env. Used when adding variables via export.
** path: 't_path' struct with parsed path info.
*/
typedef struct s_data
{
	char		*input;
	int			pipes;
	t_exec		*exec;
	t_list		*token_list;
	t_list		*env;
	char		**env_tab;
	int			nb_env;
	t_path		path;
}				t_data;

/* MAIN */
void	print_all(t_data *data);

/* SPLIT LIST */
int		is_metacharacter(char c);
int		is_space(char c);
char	*fill_tmp(char *str, int len);
void	add_node(t_data *data, char *str, int i, int j);
void	add_node_double_quote(t_data *data, char *str, int i, int j);
void	add_node_single_quote(t_data *data, char *str, int i, int j);
void	split_in_list(t_data *data, char *str);

/* PARSE COMMANDES */
void	assign_type(t_data *data);
void	parse_cmd(t_data *data);
int		is_redirection(t_list *tmp);

// /* ENV VARIABLES */
// void	assign_variable_name(t_data *data, t_list *current);

/* COUNT */
int		count_cmd(t_list *tmp);
void	count_pipes(t_data *data);
void	count_redirections(t_list *tmp, t_exec *current, int x);
void	init_exec(t_exec *current, int x);

/* REDIRECTIONS */
void	fill_files(t_data *data);

/* PATH */
void	parse_path(t_data *data);

/* ENV */
void	fill_env_list(char **env, t_data *data);
void	print_env_tab(t_data *data);

/* EXPORT */
char	*extract_name(t_data *data, char *variable);
void	ft_sort(t_list *tmp, t_list *print);
void	builtin_export(t_data *data, char **pos);

/* ECHO */
void	print_lines(int i, char **str, int fd);
void	builtin_echo_str(t_data *data, char **cmd);

/* UNSET */
void	builtin_unset(t_data *data, t_list *pos);

/* EXIT */
void	clear_cmd(t_data *data);
void	free_tab(char **tab);
void	exit_all(t_data *data, int err, char *str);
#endif
