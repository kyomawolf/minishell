/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 14:11:34 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/22 18:34:04 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	size_t	len;
	char	*rc;

	if (s == NULL)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	rc = malloc(sizeof(char) * len + 1);
	if (rc == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		rc[i] = f(i, s[i]);
		i++;
	}
	rc[i] = '\0';
	return (rc);
}
