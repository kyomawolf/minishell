/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DEBUG_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 18:31:46 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/02 20:30:14 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
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
				print_command(prin->child[i]->command);
			}
			i++;
		}
	}
}
