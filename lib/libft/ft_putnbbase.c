/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbbase.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 15:53:04 by jkasper           #+#    #+#             */
/*   Updated: 2021/07/13 22:59:53 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	ft_putnbbase(long long nb, char *base, int len)
{
	int			c;
	long long	o;
	int			temp;

	o = nb;
	if (o < 0)
	{
		c = '-';
		write(1, &c, 1);
		len++;
		o *= (-1);
	}
	if ((o / ft_strlen(base)) != 0)
		temp = ft_putnbbase(o / ft_strlen(base), base, len + 1);
	else
		temp = len;
	c = (o % ft_strlen(base));
	write(1, &base[c], 1);
	return (temp);
}
