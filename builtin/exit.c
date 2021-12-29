/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 10:45:42 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/29 16:40:43 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "stdio.h"
#include "libft.h"
#include "exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	exit_main(char **argv, t_data *data, t_node *head)
{
	int	ret;

	ret = 0;
	printf("Goodbye %s!\n", mini_getenv(data, "USER"));
	if (argv[1] != NULL && argv[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	else if (argv[1] != NULL && ft_isallnum(argv[1]) == 0)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ret = 255;
	}
	else if (argv[1] != NULL)
		ret = ft_atoi(argv[1]);
	free_main(data);
	ft_free_pipeline(&head);
	free_char_array(&argv);
	//system("leaks -quiet minishell");
	exit(ret);
}
