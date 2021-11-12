/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DEBUG_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 18:31:46 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/12 18:45:29 by jkasper          ###   ########.fr       */
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
	if (com->arguments == NULL)
		printf("| (NULL)\n");
	else
		printf("| %s\n", com->arguments[com->command]);
}

void	print_binary_tree(b_tree *prin, int dep)
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
				print_command(prin->child[i]->com);
			}
			i++;
		}
	}
}
