/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 18:39:40 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/25 19:58:22 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_checkinfront(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\f')
			i++;
		else if (str[i] == '\t' || str[i] == '\r' || str[i] == '\v')
			i++;
		else if (str[i] == '-')
			return (-(i + 1));
		else if (str[i] == '+')
			return (i + 1);
		else
			return (i);
	}
	return (i);
}

int	ft_atoi(const char *str)
{
	long	i;
	long	reval;
	int		neg;

	neg = 1;
	reval = 0;
	i = ft_checkinfront(str);
	if (i < 0)
	{	
		neg = -1;
		i *= (-1);
	}
	while (str[i] != '\0')
	{
		if (str[i] <= '9' && str[i] >= '0')
		{
			reval = (str[i] - 48) + reval * 10;
		}
		else
			return (reval * neg);
		i++;
	}
	return ((int)(reval * neg));
}
