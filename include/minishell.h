/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 13:29:02 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 14:56:28 by mrabourd         ###   ########.fr       */
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

typedef struct	s_exec
{
	char		**cmd; 		/* tab de commandes entre chaque pipe */
	char		**infile;	/* nom de chaque infile entre chaque pipe */
	char		**outfile;	/* nom de chaque outfile entre chaque pipe */
	int			fdin;		/* fdin, par defaut 0 */
	int			fdout;		/* par defaut 1 */
	int			redirect_input;		/* nombre de '<' entre chaque pipe */
	int			redirect_output;	/* nombre de '>' */
	int			heredoc;			/* nombre de '<< */
	int			delimiter_append;	/* nombre de '>>' */
	int			nb_cmd;				/* nombre de commandes dans le tab cmd */
}				t_exec;

typedef struct s_data
{
	char		*input;
	int			pipes;			/* nombre de pipes + 1 */
	t_exec		*exec;			/* tableau de structures 't_exec' en fonction du nombre de pipes + 1 */
	t_list		*token_list;	/* input tranforme en liste chainee (voir le .h de la libft )*/
	t_list		*env;			/* liste chainee env */
	char		**env_tab;		/* env en tableau de char */
	int			nb_env;			/* nb de variable dans env, au cas ou nouvelle variable avec export */
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
void    count_redirections(t_list *tmp, t_exec *current, int x);
void    init_exec(t_exec *current, int x);

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
void		builtin_echo_str(t_data *data, char **cmd);

/* UNSET */
void	builtin_unset(t_data *data, t_list *pos);

/* EXIT */
void	clear_cmd(t_data *data);
void	free_tab(char **tab);
void	exit_all(t_data *data, int err, char *str);

#endif
