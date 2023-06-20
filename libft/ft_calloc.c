/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:12:07 by mrabourd          #+#    #+#             */
/*   Updated: 2022/11/22 13:18:59 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*s;

	if (nmemb == 0 || size == 0)
	{
		s = malloc(1);
		if (s == 0)
			return (0);
		s[0] = 0;
	}
	if (nmemb > 2147483647 || size > 2147483647 || size * nmemb > 2147483647)
		return (0);
	s = malloc(size * nmemb);
	if (s == 0)
		return (0);
	ft_bzero(s, (size * nmemb));
	return (s);
}
