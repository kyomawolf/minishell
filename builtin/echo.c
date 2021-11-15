/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:47:07 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/15 17:22:38 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "minis.h"
#include "libft.h"

int	echo_main(int argc, char **argv, int fd)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	if (argc > 1 && ft_strlen(argv[1]) == 2)
	{
		if (argv[1][0] == '-' && argv[1][1] == 'n' && argv[0][2] == '\0')
			nl = 0;
	}
	else
	{
		ft_putstr_fd(argv[i++], fd);
		ft_putchar_fd(' ', fd);
	}
	while (i <= argc)
	{
		ft_putstr_fd(argv[i], fd);
		i++;
		if (i <= argc)
			ft_putchar_fd(' ', fd);
	}
	if (nl)
		ft_putchar_fd('\n', fd);
	return (0);
}
