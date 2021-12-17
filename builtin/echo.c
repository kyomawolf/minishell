/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:47:07 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 23:13:18 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "minis.h"
#include "libft.h"

int	echo_main(char **argv)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	if (ft_char_arr_len(argv) > 1 && ft_strlen(argv[1]) == 2)
	{
		if (argv[1][0] == '-' && argv[1][1] == 'n' && argv[0][2] == '\0')
			nl = 0;
	}
	else
	{
		ft_putstr_fd(argv[i++], 1);
		ft_putchar_fd(' ', 1);
	}
	while (i <= ft_char_arr_len(argv))
	{
		ft_putstr_fd(argv[i], 1);
		i++;
		if (i <= ft_char_arr_len(argv))
			ft_putchar_fd(' ', 1);
	}
	if (nl)
		ft_putchar_fd('\n', 1);
	return (0);
}
