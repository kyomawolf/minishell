/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 11:27:24 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/29 17:08:32 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static char	**ft_freeman(char **restr, size_t count)
{
	size_t	i;

	i = count;
	while (restr[i] != NULL)
	{
		free(restr[i]);
		i++;
	}
	free(restr);
	return (NULL);
}

static char	**ft_callsplit(char *s, size_t count, size_t i, size_t ii)
{
	char	**rs;

	rs = ft_calloc(count + 1, sizeof(char *));
	if (rs == NULL)
		return (NULL);
	if (ii >= i + 1)
	{
		rs[count - 1] = ft_strdup(s + i);
		if (rs[count - 1] == NULL)
			return (ft_freeman(rs, count));
	}
	return (rs);
}

static char	**ft_recsplit(char *s, char c, size_t count)
{
	size_t	i;
	size_t	ii;
	char	**rs;

	i = 0;
	while (s[i] != '\0' && s[i] == c)
		i++;
	ii = i;
	while (s[ii] != c && s[ii] != '\0')
		ii++;
	count++;
	if (i >= ft_strlen(s) || ii >= ft_strlen(s))
	{
		rs = ft_callsplit(s, count, i, ii);
		if (rs == NULL)
			return (NULL);
		return (rs);
	}
	rs = ft_recsplit(s + ii, c, count);
	if (rs == NULL)
		return (NULL);
	rs[count - 1] = ft_substr(s, i, ii - i);
	if (rs[count - 1] == NULL)
		return (ft_freeman(rs, count));
	return (rs);
}

char	**ft_split(char const *s, char c)
{
	if (s == NULL)
		return (NULL);
	return (ft_recsplit(((char *)s), c, 0));
}
