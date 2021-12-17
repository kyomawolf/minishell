/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:07:37 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 16:24:39 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minis.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>

t_bin	*add_com(t_node **ori_node, t_bin *parent)
{
	t_bin	*ret;
	t_e_op	token;

	token = ((t_token *)(*ori_node)->content)->type;
	ret = add_node(parent, *ori_node);
	if (token < OPAR)
	{
		*ori_node = (*ori_node)->next;
		token = ((t_token *)(*ori_node)->content)->type;
	}
	while (*ori_node != NULL)
	{
		if (token > CPAR && token < QUOTE)
			*ori_node = add_io(ret, *ori_node);
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
		if (token == OPAR || (token == WORD && sw == 1))
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

	if (check_input(head))
	{
		write(1, "\nERROR\n", 7);
		return (NULL);
	}
	cpy = head;
	tree = b_tree_init(&cpy, 0);
	//print_binary_tree(tree, 3);
	free_t_node_list(head);
	return (tree);
}
