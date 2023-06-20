/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 16:08:46 by mrabourd          #+#    #+#             */
/*   Updated: 2022/11/19 15:43:14 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	res;

	i = 0;
	if (size == 0 && (!src || !dst))
		return (0);
	while (dst[i] && i < size)
		++i;
	res = 0;
	while (src[res])
		++res;
	if (size == i)
		return (res + size);
	else
		res += i;
	j = 0;
	while (src[j] && (i + 1) < size)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (res);
}

/*
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	j;
	int		len;

	if(size == 0 && (!src || !dst))
		return (0);
	len = ft_strlen(dst);
	j = 0;
	if (size <= ft_strlen(dst))
		return (ft_strlen((char *)src) + size);
	while (src[j] && j + len < size - 1)
	{
		dst[j + len] = src[j];
		j++;
	}
	dst[j + len] = 0;
	if (size < (size_t)ft_strlen(dst))
		return (ft_strlen((char *)src) + size);
	else
		return (len + ft_strlen((char *)src));
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	len_dst;
	size_t	len_src;

	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	i = 0;
	j = len_dst;
	if (!dst && !src)
		return (0);
	if (len_dst < size - 1 && size > 0)
	{
		while (src[i] && dst[j] && len_dst + i < size - 1)
		{
			dst[j] = src[i];
			i++;
			j++;
		}
		dst[j] = 0;
	}
	if (len_dst >= size)
		len_dst = size;
	return (len_dst + len_src);
}

size_t	ft_error(char *dst, size_t size)
{
	if (size > ft_strlen(dst))
		return (ft_strlen(dst));
	else
		return (size);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	if (!(size && dst))
		return (ft_strlen(src));
	if (!src)
		return (ft_error(dst, size));
	i = 0;
	j = 0;
	while (dst[i] && i < size)
		i++;
	while (i + j < (size - 1) && src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	// if ((i + j) == size - 1)
	dst[i + j] = '\0';
	while (src[j])
		j++;
	return (i + j);
}
*/