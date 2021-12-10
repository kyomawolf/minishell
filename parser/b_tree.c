/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:48:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/10 17:42:59 by mstrantz         ###   ########.fr       */
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

	root = ft_calloc(1, sizeof(t_bin));
	root->depth = depth;
	if ((t_node *)((*node)->prev) != NULL)
		root->control_op = ((t_token *)(*node)->content)->type;
	else
		root->control_op = add_last_operator(*node);
	root->child_amount = count_children(*node);
	printf("children: %i\n", root->child_amount);
	root->child = ft_calloc(root->child_amount + 1, sizeof(t_bin *));
	while (1)
	{
		if (b_tree_add_child(node, root))
			break ;
	}
	return (root);
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
		printf("test\n");
		ret = 0;
		while (((t_token *)(*node)->content)->type == CPAR)
		{
			if ((*node)->next == NULL)
				return (1);
			(*node) = (*node)->next;
		}
		root->child[i] = add_com(node, root);//determines stage and fills comms
		printf("op-n: %i\n", root->child[i]->control_op);
		if (root->child[i] == NULL)
		{
			*node = (*node)->next;
			root->child[i] = b_tree_init(node, root->depth + 1);
			printf("opc: %i\n", root->child[i]->control_op);
		}
		i++;
	}
	return (ret);
}
