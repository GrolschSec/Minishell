/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 11:21:21 by mrabourd          #+#    #+#             */
/*   Updated: 2022/11/14 17:58:49 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	t;
	char	*str;

	str = (void *)s;
	t = 0;
	while (t < n)
	{
		if (str[t] == (char)c)
			return (&str[t]);
		t++;
	}
	return (0);
}
