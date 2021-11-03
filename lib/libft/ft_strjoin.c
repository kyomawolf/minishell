/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 10:25:07 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/21 13:08:42 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	lens1;
	size_t	lens2;
	char	*s3;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	s3 = ft_calloc((lens1 + lens2 + 1), 1);
	if (s3 == NULL)
		return (NULL);
	ft_memcpy(s3, s1, lens1);
	ft_strlcat(s3, s2, lens1 + lens2 + 1);
	return (s3);
}
