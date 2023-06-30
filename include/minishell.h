/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 13:29:02 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/30 16:55:34 by mrabourd         ###   ########.fr       */
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
# include <limits.h>
# include <sys/wait.h>
# include "../libft/libft.h"
# include <errno.h>
# define COLOR_RESET "\e[0;37m"
# define COLOR_RED "\e[1;31m"

enum e_buitin
{
	NOT_BUILTIN,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
};

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
	t_list		*outfile;
	char		**eof;
	int			fdin;
	int			fdout;
	int			redirect_input;
	int			redirect_output;
	int			heredoc;
	int			is_last;
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
	char		*str;
	int			pipes;
	int			cpy_in;
	int			cpy_out;
	int			*exit_code;
	t_exec		*exec;
	t_list		*token_list;
	t_list		*env;
	char		**env_tab;
	int			nb_env;
	t_path		path;
	char		*tilde;
}				t_data;

/* PRINT ----  TO REMOVE */
void	print_tab(t_data *data);
void	print_all(t_data *data);

/* MAIN */
void	print_all(t_data *data);
void	ft_handler(int sig);

/* SPLIT LIST */
int		is_meta(char c);
int		is_space(char c);
char	*fill_tmp(char *str, int len);
void	add_node(t_data *data, int i, int j);
void	split_in_list(t_data *data);
void	add_in_previous_node(t_data *data, int *i, int *j);
void	split_quote(t_data *data, int *i, int *j, char quotetype);

/* PARSE COMMANDES */
int		is_redirection(t_list *tmp);
void	fill_exec(t_data *data, t_list **tmp, t_exec *current, int x);
void	put_cmd_in_tab(t_data *data, int nb);
void	parse_cmd(t_data *data);

/* ASSIGN TYPES */
void	len_is_one(t_data *data, t_list *tmp);
void	len_is_two(t_list *tmp);
void	type_dollar(t_data *data, t_list *tmp);
void	type_option(t_list *tmp);
void	assign_type(t_data *data);

// /* ENV VARIABLES */
// void	assign_variable_name(t_data *data, t_list *current);

/* COUNT */
int		count_cmd(t_list *tmp);
void	count_pipes(t_data *data);
void	count_redirections(t_list *tmp, t_exec *current, int x);
void	init_exec(t_exec *current, int x);

/* REDIRECTIONS */
void	fill_files(t_data *data);
void	open_files(t_data *data);

/* HEREDOC / EOF */
void	fill_eof(t_data *data);

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
void	close_fds(t_data *data);
void	free_env(t_data *data);

/* EXEC_1 */
void	execution(t_data *data);
int		process_creation(t_data *data, t_exec *exec);
int		command_exec(t_data *data, t_exec *exec);
int		exec_last_child(t_data *data, t_exec *exec);
int		last_child(t_data *data, t_exec *exec);

/* EXEC_2 */
int		is_builtin(char *cmd);
char	*get_cmd_path(char *cmd, t_data *data);
void	select_builtin(t_data *data, t_exec *exec);
void	execution_handling(t_data *data, int i);

/* EXEC_UTILS_1 */
char	*ft_strjoin2(char *s1, char const *s2);
void	end_exec(t_data *data);
void	exit_ps(t_data *data, int error);
void	exec_error(char *name, char *str);
void	exit_minishell(t_data *data);

/* EXEC_UTILS_2 */
long long	ft_atoll(const char *str);
int			is_out_of_range(const char *str);

/*BUILTIN EXIT*/
void	exit_builtin(t_data *data, t_exec *exec);
int		str_isdigit(char *str);
int		convert_to_exit_code(long long nb);
void	numeric_arg_error(t_exec *exec);
void	perform_exit(int ex, t_data *data, t_exec *exec);

/* BUILTIN CD */
void	cd_error(char *path);
void	cd_builtin(t_data *data, t_exec *exec);
void	cd_no_arg_case(t_data *data, t_exec *exec);
void	cd_arg_case(t_data *data, t_exec *exec);

/* BUILTIN_EXPORT */
void	ft_setenv(t_data *data, char *name, char *value);
char	*make_new_var(char *name, char *value);
void	export_builtin(t_data *data, t_exec *exec);
int		parse_export(char *var);
char	**split_export(char *arg);

/* BUILTIN ECHO */
void	echo_builtin(t_exec *exec);

/* BUILTIN PWD */
void	pwd_builtin(void);

/* BUILTIN UTILS */
char	*ft_getenv(t_data *data, char *name);
char	*get_value(t_list *env);

/* BUILTIN ERROR */
void	invalid_option(char c);
void	not_valid_identifier(char *arg);
void	print_usage(void);
void	export_no_arg(t_data *data);
#endif
