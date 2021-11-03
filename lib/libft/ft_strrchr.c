/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 16:11:27 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/17 15:31:42 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*p;
	int		i;

	i = 0;
	p = 0;
	while (1)
	{
		if (s[i] == c)
			p = (char *)(s + i);
		if (s[i] == '\0')
			return (p);
		i++;
	}
}
