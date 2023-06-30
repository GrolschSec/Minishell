/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:12:49 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/06/30 18:39:25 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   Updated: 2023/06/30 18:26:08 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Comportement export:
 * 		- export: sans arguments affiche le contenu de l'envirronement
 * 			d'une certaine facon
 *		- export HELLO="Hello World": update la var si elle existe 
 *			et ajoute une nouvelle variable si elle n'existe pas
 *		- export name: ne fais rien pour creer une variable on doit avoir
 *			un signe egual.
 *		Un nom de variable valide:
 *			- Underscore: OK
			- Tiret: NOK premiere position (bash: export: -_: invalid option
				export: usage: export [-fn] [name[=value] ...] or export -p) NOK
 *			- LETTRE MAJ/MIN : OK
			- Chiffres : OK mais pas en premiere position
			
 *		
**/
//bash: export: -_: invalid option
//bash: export: `_-=': not a valid identifier

void	ft_setenv(t_data *data, char *name, char *value)
{
	t_list	*tmp;
	t_list	*env;
	char	*tmp_str;
	char	*new_env;

	env = NULL;
	tmp = data->env;
	new_env = make_new_var(name, value);
	while (tmp)
	{
		if (ft_strncmp(name, tmp->content, ft_strlen(name)) == 0)
		{
			env = tmp;
			break ;
		}
		tmp = tmp->next;
	}
	if (!env)
<<<<<<< HEAD
		ft_lstadd_back(&data->env, ft_lstnew(new_env));
=======
	{
		ft_lstadd_back(&data->env, ft_lstnew(new_env));
		free(new_env);
	}
>>>>>>> main
	else
	{
		tmp_str = env->content;
		env->content = new_env;
		free(tmp_str);
	}
}

char	*make_new_var(char *name, char *value)
{
	char	*new_env;

	new_env = ft_strjoin2(name, value);
	if (!new_env)
		return (free(value), NULL);
	return (free(value), new_env);
}

int	parse_export(char *var)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=')
	{
		if (i == 0 && var[i] == '-' && var[i + 1])
			return (invalid_option(var[i + 1]), 0);
		else if (i == 0 && !ft_isalpha(var[i]))
			return (not_valid_identifier(var), 0);
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (not_valid_identifier(var), 0);
		i++;
	}
	if (var[i] != '=')
		return (0);
	return (1);
}

char	**split_export(char *arg)
{
	int		len;
	char	**cmd;

	cmd = malloc(sizeof(char *) * 3);
	if (!cmd)
		return (NULL);
	len = 0;
	while (arg[len] != '=')
		len++;
	len++;
	cmd[0] = malloc(sizeof(char) * (len + 1));
	if (!cmd[0])
		return (free(cmd), NULL);
	ft_strlcpy(cmd[0], arg, len + 1);
	cmd[1] = malloc(sizeof(char) * ((ft_strlen(arg) - len) + 2));
	if (!cmd[1])
		return (free(cmd[0]), free(cmd), NULL);
	ft_strlcpy(cmd[1], &arg[len], ft_strlen(arg) - len + 1);
	cmd[2] = NULL;
	return (cmd);
}

void	export_builtin(t_data *data, t_exec *exec)
{
	int		i;
	char	**cmd;

	if (exec->nb_cmd == 1)
		export_no_arg(data);
	else
	{
		i = 1;
		while (i < exec->nb_cmd)
		{
			if (parse_export(exec->cmd[i]) && !(data->pipes > 2))
			{
				cmd = split_export(exec->cmd[i]);
				if (!cmd)
					return ;
				ft_setenv(data, cmd[0], cmd[1]);
				free(cmd[0]);
				free(cmd);
			}
			i++;
		}
	}
}
