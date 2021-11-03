/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 12:58:05 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/25 19:59:08 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	lendst;
	size_t	lensrc;

	lendst = ft_strlen(dst);
	lensrc = ft_strlen(src);
	i = 0;
	if (lendst >= dstsize)
		return (dstsize + lensrc);
	while (i < dstsize - lendst - 1 && src[i] != '\0')
	{
		dst[i + lendst] = src[i];
		i++;
	}
	if (dstsize != 0 && dstsize >= lendst)
		dst[i + lendst] = '\0';
	return (lendst + lensrc);
}
