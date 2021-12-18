/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 10:45:42 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/18 12:18:24 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "stdio.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	exit_main(char **argv, t_data *data)
{
	/* if (argv[1] != NULL)
		return (42); */
	printf("Goodbye %s!\n", mini_getenv(data, "USER"));
	free_main(data);
	free_char_array(&argv);
	system("leaks -quiet minishell");
	exit(0);
}
