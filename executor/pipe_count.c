/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 16:51:06 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/23 22:50:37 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include <stdlib.h>
#include <stdio.h>

int	pipe_next_child(int i, t_bin *tree)
{
	if (tree->child == NULL || tree->child[i] == NULL)
		return (0);
	if (tree->child[i]->child != NULL)
		return (-1);
	if (tree->child[i]->control_op == PIPE)
		return (1);
	else
		return (0);
}

int	pipe_count_main(t_bin *tree, int i)
{
	int	count;

	count = 1;
	while (pipe_next_child(i, tree))
	{
		count += pipe_next_child(i, tree);
		i++;
	}
	return (count);
}
