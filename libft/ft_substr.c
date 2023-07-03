/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 20:40:00 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/03 13:35:29 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_malloc(size_t len_new)
{
	char	*new;

	new = 0;
	new = malloc(sizeof(char) * len_new);
	if (new == NULL)
		return (NULL);
	return (new);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	len_new;

	len_new = 1;
	if (start > ft_strlen(s) || !s || !len || len < 1)
	{
		new = malloc(1);
		if (new == 0)
			return (NULL);
		new[0] = '\0';
		return (new);
	}
	if (len > ft_strlen(s))
		len_new = ft_strlen(s) + 1;
	if (len < ft_strlen(s))
		len_new = len + 1;
	if (len == ft_strlen(s))
		len_new = ft_strlen(s) - start + 1;
	new = ft_malloc(len_new);
	if (new == 0)
		return (NULL);
	ft_strlcpy(new, &s[start], len_new);
	return (new);
}
