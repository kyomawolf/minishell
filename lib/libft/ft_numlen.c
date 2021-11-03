/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 17:54:31 by jkasper           #+#    #+#             */
/*   Updated: 2021/07/14 18:59:47 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_numlen(long nb, int base)
{
	long	i;

	i = 1;
	if (nb < 0)
	{
		nb *= -1;
		i++;
	}
	while (1)
	{
		if (nb < base)
			return (i);
		nb = nb / base;
		i++;
	}
}
