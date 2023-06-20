/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 19:24:03 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/06 00:34:22 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;
	int		y;
	int		len;

	i = 0;
	y = 0;
	len = ft_strlen(s) + 1;
	dup = malloc(sizeof(char) * len);
	if (dup == 0 || !dup)
		return (NULL);
	if (s)
	{
		while (s[i])
		{
			dup[y] = s[i];
			i++;
			y++;
		}
	}
	dup[y] = '\0';
	return (dup);
}
