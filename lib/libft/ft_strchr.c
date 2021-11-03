/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 13:48:29 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/21 12:42:30 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*p;
	int		i;

	i = 0;
	p = NULL;
	while (1)
	{
		if (s[i] == c)
			return ((char *)(s + i));
		if (s[i] == '\0')
			return (p);
		i++;
	}
}
