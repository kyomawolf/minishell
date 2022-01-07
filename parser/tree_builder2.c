/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:07:44 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/07 17:34:18 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minis.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>

int	count_words(t_node *node)
{
	int	counter;

	counter = 0;
	while (node != NULL && ((t_token *)node->content)->type == WORD)
	{
		counter++;
		node = node->next;
	}
	return (counter);
}

t_e_op	add_last_operator(t_node *node)
{
	while (node->prev != NULL)
	{
		if (((t_token *)node->content)->type < OPAR)
			return (((t_token *)node->content)->type);
		node = node->prev;
	}
	return (0);
}

t_bin	*add_node(t_bin *par, t_node *node)
{
	t_bin	*ret;

	ret = ft_calloc(1, sizeof(t_bin));
	if (ret == NULL)
		return (NULL);
	ret->command = ft_calloc(1, sizeof(t_simple_com));
	ret->parent = par;
	ret->control_op = add_last_operator(node);
	if (ret->command != NULL)
		return (ret);
	free(ret);
	return (NULL);
}

void	move_bracket(t_node **ori_node)
{
	while (((t_token *)(*ori_node)->content)->type != OPAR)
		*ori_node = (t_node *)(*ori_node)->next;
}

t_bin	*bracket_check(t_node **ori_node, t_bin *parent)
{
	if ((t_node *)(*ori_node)->next == NULL)
		return (add_com(ori_node, parent));
	else if (((t_token *)((t_node *)(*ori_node)->next)->content)->type == OPAR)
	{
		move_bracket(ori_node);
		return (NULL);
	}
	else if (((t_token *)(*ori_node)->content)->type == OPAR)
	{
		move_bracket(ori_node);
		return (NULL);
	}
	else
		return (add_com(ori_node, parent));
}
