/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 10:45:42 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/29 01:35:11 by mstrantz         ###   ########.fr       */
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
	if (argv[1] != NULL && argv[2] != NULL)
		printf("Too many arguments!\n");
	else if (argv[1] != NULL && ft_isallnum(argv[1]) == 0)
		printf("Exitcode must be numerical!\n");
	else if (argv[1] != NULL)
		ret = ft_atoi(argv[1]);
	printf("Goodbye %s!\n", mini_getenv(data, "USER"));
	free_main(data);
	ft_free_pipeline(&head);
	free_char_array(&argv);
	system("leaks -quiet minishell");
	exit(ret);
}
