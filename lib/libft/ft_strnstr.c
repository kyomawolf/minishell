/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 16:18:39 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/21 12:17:59 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, unsigned long len)
{
	unsigned long	i;
	unsigned long	ii;

	i = 0;
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (i < len && haystack[i] != '\0')
	{
		if (haystack[i] == needle[0])
		{
			ii = 0;
			while (needle[ii] != '\0' && (ii + i) < len)
			{
				if (haystack[ii + i] != needle[ii])
					break ;
				ii++;
			}
			if (needle[ii] == '\0')
				return ((char *)haystack + i);
		}
		i++;
	}
	return (NULL);
}
