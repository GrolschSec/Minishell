/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 19:19:04 by mrabourd          #+#    #+#             */
/*   Updated: 2022/11/21 19:04:56 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_signe(int i)
{
	int	signe;

	signe = 0;
	if (i < 0)
		signe = 1;
	return (signe);
}

static int	ft_nblen(unsigned int i)
{
	int	count;

	count = 1;
	while (i > 9)
	{
		count++;
		i /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char			*nb;
	size_t			len;
	unsigned int	u;
	int				signe;

	u = n;
	signe = ft_signe(n);
	if (signe)
		u = -n;
	len = ft_nblen(u);
	nb = malloc(sizeof(char) * (len + 1 + signe));
	if (nb == 0)
		return (NULL);
	if (signe)
		nb[0] = '-';
	nb[len + signe] = '\0';
	while (len > 0)
	{
		nb[--len + signe] = (u % 10) + '0';
		u /= 10;
	}
	return (nb);
}
