/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 12:09:49 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/25 19:59:41 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static int	ft_intlen(int n)
{
	int	count;

	count = 0;
	while (n != 0)
	{
		n = (n / 10);
		count++;
	}
	if (n < 0)
		count++;
	return (count);
}

char	*ft_itoa(int n)
{
	long	cpy;
	int		count;
	char	*rc;

	cpy = n;
	count = ft_intlen(n);
	if (n < 0)
		cpy = cpy * (-1);
	rc = ft_calloc((count + 2), sizeof(char));
	if (rc == NULL)
		return (NULL);
	if (n == 0)
		rc[0] = '0';
	if (n > 0)
		count--;
	while (cpy != 0)
	{
		rc[count] = (cpy % 10) + '0';
		cpy = cpy / 10;
		count--;
	}
	if (n < 0)
		rc[0] = '-';
	return (rc);
}
