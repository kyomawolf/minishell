/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DEBUG_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 18:31:46 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/23 22:55:51 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include <stdio.h>

void	print_chars(int amount, char c)
{
	while (amount > 0)
	{
		printf("%c", c);
		amount--;
	}
}

void	print_command(t_simple_com *com)
{
	int	i;

	i = 0;
	printf("|");
	if (com->arguments == NULL)
		printf(" (NEXT)");
	else
	{
		while (com->arguments[i] != NULL)
			printf(" %s ", com->arguments[i++]);
	}
	printf("\n");
}

void	print_binary_tree(t_bin *prin, int dep)
{
	int	i;

	i = 0;
	if (prin->child != NULL)
	{
		while (prin->child[i] != NULL)
		{
			if (prin->child[i]->child != NULL)
				print_binary_tree(prin->child[i], dep + 1);
			else
			{
				print_chars(dep, '\t');
				if (prin->child[i]->command->arguments == NULL)
					printf("NULL\n");
				else
					print_command(prin->child[i]->command);
			}
			i++;
		}
	}
}
