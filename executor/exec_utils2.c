/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 19:55:46 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/08 15:02:37 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <signal.h>
#include "libft.h"

void	child_signal(int signal)
{
	if (signal == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", 2);
}
