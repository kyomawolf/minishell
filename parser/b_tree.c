/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:48:13 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/07 17:20:02 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minis.h"
#include "parser.h"
#include "struct.h"
#include <stdio.h>

int		b_tree_add_child(t_node **node, t_bin *root);

//recursive with b_tree_add_child();
t_bin	*b_tree_init(t_node **node, int depth)
{
	t_bin	*root;
	int		ret;

	root = ft_calloc(1, sizeof(t_bin));
	if (root == NULL)
		return (NULL);
	root->depth = depth;
	if ((t_node *)((*node)->prev) != NULL)
		root->control_op = ((t_token *)(*node)->content)->type;
	else
		root->control_op = add_last_operator(*node);
	root->child_amount = count_children(*node);
	root->child = ft_calloc(root->child_amount + 1, sizeof(t_bin *));
	if (root->child == NULL)
	{
		free_tree(root);
		return (NULL);
	}
	while (1)
	{
		ret = b_tree_add_child(node, root);
		if (ret == 1)
			break ;
		else if (ret == 2)
		{
			free_tree(root);
			return (NULL);
		}
	}
	return (root);
}

int	b_tree_add_child_skip_cpars(t_node **node)
{
	while (((t_token *)(*node)->content)->type == CPAR)
	{
		if ((*node)->next == NULL)
			return (1);
		(*node) = (*node)->next;
	}
	return (0);
}

//recursive with b_tree_init();
int	b_tree_add_child(t_node **node, t_bin *root)
{
	int	i;
	int	ret;

	ret = 1;
	i = 0;
	while (root->child[i] != NULL)
		i++;
	while (i < root->child_amount)
	{
		ret = 0;
		if (b_tree_add_child_skip_cpars(node))
			return (1);
		root->child[i] = bracket_check(node, root);
		if (root->child[i] == NULL)
			*node = (*node)->next;
		if (root->child[i] == NULL)
			root->child[i++] = b_tree_init(node, root->depth + 1);
		else if (root->child[i++] == (void *)1)
		{
			root->child[i - 1] = NULL;
			return (2);
		}
	}
	return (ret);
}
