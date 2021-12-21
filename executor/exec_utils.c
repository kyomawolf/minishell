/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 18:47:45 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 19:39:35 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <signal.h>

char	ft_tolower2(unsigned int i, char c)
{
	(void)i;
	return (ft_tolower(c));
}

void	wait_child(int sig)
{
	(void) sig;
	return ;
}

void	ft_signals(void)
{
	signal(SIGQUIT, wait_child);
	signal(0, wait_child);
	signal(1, wait_child);
	signal(2, wait_child);
	signal(3, wait_child);
	signal(4, wait_child);
	signal(5, wait_child);
	signal(6, wait_child);
	signal(7, wait_child);
	signal(11, wait_child);
	signal(13, wait_child);
}
