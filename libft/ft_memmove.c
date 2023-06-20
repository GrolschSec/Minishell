/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 19:41:15 by mrabourd          #+#    #+#             */
/*   Updated: 2022/11/19 15:43:51 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *str1, const void *str2, size_t n)
{
	char		*s1;
	const char	*s2;

	s1 = str1;
	s2 = str2;
	if (str1 == 0 && str2 == 0)
		return (str1);
	if (str1 < str2)
	{
		while (n-- > 0)
			*s1++ = *s2++;
	}
	if (str1 > str2)
	{
		while (n > 0)
		{
			s1[n - 1] = s2[n - 1];
			n--;
		}
	}
	return (str1);
}
