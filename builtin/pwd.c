/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 16:56:33 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/28 23:15:12 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minis.h"
#include "parser.h"
#include "struct.h"
#include <stdio.h>
#include <unistd.h>

int	pwd_main(void)
{
	char	*cwd;
	int		es;

	es = 0;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		cwd = "minishell: pwd: could not determine current directory\n";
		ft_putstr_fd(cwd, 2);
		es = 1;
	}
	else
	{
		printf("%s\n", cwd);
		free (cwd);
	}
	return (es);
}
