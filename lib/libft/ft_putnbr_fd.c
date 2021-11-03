/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 16:11:42 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/20 19:24:08 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;
	long	o;

	o = n;
	if (o < 0)
	{
		c = '-';
		write(fd, &c, 1);
		o *= (-1);
	}
	if ((o / 10) != 0)
		ft_putnbr_fd(o / 10, fd);
	c = (o % 10) + '0';
	write(fd, &c, 1);
}
