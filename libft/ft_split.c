/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 18:18:37 by mrabourd          #+#    #+#             */
/*   Updated: 2023/03/07 16:03:59 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_freeall(char **new)
{
	int	i;

	i = 0;
	if (!new)
		return (NULL);
	while (new[i])
	{
		free (new[i]);
		i++;
	}
	free (new);
	return (NULL);
}

static int	ft_nbs(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (!s)
	{
		exit(EXIT_FAILURE);
	}
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if ((s[i]) != c && s[i])
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static int	ft_nchar(char const *s, char c, int i)
{
	int	y;

	y = 0;
	while (s[i] == c)
		i++;
	while (s[i] != c && s[i])
	{
		y++;
		i++;
	}
	return (y);
}

static void	ft_strdup2(char c, const char *s, char *new, int i)
{	
	int	j;

	j = 0;
	while (s[i] == c)
		i++;
	while (s[i] && s[i] != c)
	{
		new[j] = s[i];
		i++;
		j++;
		if (s[i] == c)
			break ;
	}
	new[j] = '\0';
}

char	**ft_split(char const *s, char c)
{
	int		nb_s;
	char	**new;
	int		i;
	int		tab;

	nb_s = ft_nbs(s, c);
	new = malloc(sizeof(char *) * (nb_s + 1));
	if (new == NULL)
		return (ft_freeall(new));
	i = 0;
	tab = 0;
	while (tab < nb_s)
	{
		new[tab] = malloc(sizeof(char) * (ft_nchar(s, c, i) + 1));
		if (new[tab] == NULL)
			return (ft_freeall(new));
		ft_strdup2(c, s, new[tab], i);
		tab++;
		while (s[i] == c)
			i++;
		i += ft_nchar(s, c, i);
	}
	new[tab] = NULL;
	return (new);
}
