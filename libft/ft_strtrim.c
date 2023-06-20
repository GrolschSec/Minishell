/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 17:35:13 by mrabourd          #+#    #+#             */
/*   Updated: 2023/05/22 17:13:44 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_ischarset(char s, char const *set)
{
	int	i;

	i = 0;
	if (!set[i])
		return (0);
	while (set[i])
	{
		if (s == set[i])
			return (1);
		i++;
	}
	return (0);
}

static char	*malloc_one(char *new, char *s1)
{
	new = malloc(sizeof(char));
	if (new == 0)
	{
		free (s1);
		return (new);
	}
	new[0] = '\0';
	free (s1);
	return (new);
}

char	*ft_strtrim(char *s1, char const *set)
{
	size_t	i;
	size_t	len_s;
	char	*new;

	i = 0;
	len_s = ft_strlen(s1);
	while (ft_ischarset(s1[i], set) && s1[i])
	{
		i++;
		if (i == len_s)
			new = malloc_one(new, s1);
	}
	while (ft_ischarset(s1[len_s - 1], set))
		len_s--;
	if (i == 0 && len_s == ft_strlen(s1))
		return (s1);
	new = malloc(sizeof(char) * (len_s - i + 1));
	if (new == 0)
	{
		free (s1);
		return (new);
	}
	ft_strlcpy(new, &s1[i], (len_s - i + 1));
	free (s1);
	return (new);
}
