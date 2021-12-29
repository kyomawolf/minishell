/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:47:07 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/29 16:11:12 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "minis.h"
#include "libft.h"

void	echo_option(char **argv, int *i, int *nl)
{
	int	j;

	while (argv[*i])
	{
		j = 0;
		if (argv[*i][j] == '-' && argv[*i][++j] == 'n')
		{
			while (argv[*i][j] != '\0')
			{
				if (argv[*i][j] != 'n')
					break ;
				j++;
			}
			if (argv[*i][j] == '\0')
				*nl = 0;
			else
				return ;
		}
		else
			return ;
		(*i)++;
	}
}

void	echo_helper(char **argv, int *i)
{
	while (*i <= ft_char_arr_len(argv))
	{
		ft_putstr_fd(argv[*i], 1);
		(*i)++;
		if (*i <= ft_char_arr_len(argv) - 1)
			ft_putchar_fd(' ', 1);
	}
}

int	echo_main(char **argv)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	echo_option(argv, &i, &nl);
	ft_putstr_fd(argv[i++], 1);
	if (i <= ft_char_arr_len(argv) - 1)
		ft_putchar_fd(' ', 1);
	echo_helper(argv, &i);
	if (nl)
		ft_putchar_fd('\n', 1);
	return (0);
}
