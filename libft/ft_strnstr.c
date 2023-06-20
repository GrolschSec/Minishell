/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 12:09:26 by mrabourd          #+#    #+#             */
/*   Updated: 2022/11/14 20:34:28 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	x;
	size_t	y;

	x = 0;
	y = 0;
	if (s2[y] == '\0')
		return ((char *)s1);
	while (x < len && s1[x])
	{
		y = 0;
		while (x < len && (s2[y] == s1[x]) && s2[y])
		{
			y++;
			x++;
			if (s2[y] == '\0')
				return ((char *)&s1[x - y]);
		}
		if (s2[y] && !s1[x] && (x >= len))
			return (0);
		x = x - y + 1;
	}
	return (0);
}
