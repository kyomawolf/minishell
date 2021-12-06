/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 16:51:06 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/06 17:26:28 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "structs.h"
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

int	pipe_count_main(t_bin *tree)
{
	int	count;
	int	i;

	count = 1;
	i = 1;
	while (pipe_next_child(i, tree))
	{
		count += pipe_next_child(i, tree);
		i++;
	}
	return (count);
}
