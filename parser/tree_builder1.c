/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:07:37 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/07 17:34:56 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minis.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>

t_bin	*add_com_helper(t_node **ori_node, t_bin *parent, t_e_op *token)
{
	t_bin	*ret;

	*token = ((t_token *)(*ori_node)->content)->type;
	ret = add_node(parent, *ori_node);
	if (ret == NULL)
		return (NULL);
	if (*token < OPAR)
		*ori_node = (*ori_node)->next;
	if (*token < OPAR)
		*token = ((t_token *)(*ori_node)->content)->type;
	return (ret);
}

t_bin	*add_com(t_node **ori_node, t_bin *parent)
{
	t_bin	*ret;
	t_e_op	token;

	ret = add_com_helper(ori_node, parent, &token);
	if (ret == NULL)
		return (NULL);
	while (*ori_node != NULL)
	{
		if (token > CPAR && token < QUOTE)
		{
			*ori_node = add_io(ret, *ori_node);
			if (*ori_node == (void *)1)
				free_tree(ret);
			if (*ori_node == (void *)1)
				return ((void *)1);
		}
		if (token < HERE_DOC)
			break ;
		*ori_node = add_words(ret->command, *ori_node);
		if (*ori_node != NULL)
			token = ((t_token *)(*ori_node)->content)->type;
	}
	return (ret);
}

t_node	*jump_pars(t_node *node)
{
	int	i;

	i = 0;
	while (node != NULL)
	{
		if (((t_token *)node->content)->type == OPAR)
			i++;
		else if (((t_token *)node->content)->type == CPAR)
			i--;
		else if (i == 0)
			return (node->prev);
		if (node->next == NULL)
			return (node);
		node = node->next;
	}
	return (node->prev);
}

int	count_children(t_node *node)
{
	int		count;
	t_e_op	token;
	int		sw;

	count = 0;
	sw = 1;
	while (node != NULL)
	{
		token = ((t_token *)node->content)->type;
		if (token == CPAR)
			break ;
		if (token < OPAR)
			sw = 1;
		if (token == OPAR || ((token == WORD || token == HERE_DOC) && sw == 1))
		{
			if (token == OPAR)
				node = jump_pars(node);
			else
				sw = 0;
			count++;
		}
		node = node->next;
	}
	return (count);
}

t_bin	*builder_main(t_node *head)
{
	t_bin	*tree;
	t_node	*cpy;

	if (check_input(head) > 0)
		return (NULL);
	cpy = head;
	tree = b_tree_init(&cpy, 0);
	return (tree);
}
