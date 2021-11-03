/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 10:48:10 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/22 13:10:17 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>

static char	*ft_cal(char *rs, size_t s1len, size_t i, char const *s1)
{
	if (i >= s1len || s1[0] == '\0')
		rs = ft_calloc(1, 1);
	else
		rs = ft_calloc(((s1len - i) + 2), 1);
	if (rs == NULL)
		return (NULL);
	return (rs);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	s1len;
	char	*rs;
	size_t	i;

	if (s1 == NULL)
		return (NULL);
	rs = NULL;
	s1len = ft_strlen(s1);
	i = 0;
	if (set != NULL)
	{
		while (s1[i] != '\0' && ft_strchr(set, s1[i]) != NULL)
			i++;
		if (i >= s1len || s1[0] == '\0')
			return (ft_cal(rs, s1len, i, s1));
		while (ft_strchr(set, s1[s1len]) != NULL)
			s1len--;
	}
	rs = ft_cal(rs, s1len, i, s1);
	if (rs == NULL)
		return (NULL);
	ft_strlcpy(rs, (s1 + i), (s1len - i) + 2);
	return (rs);
}
