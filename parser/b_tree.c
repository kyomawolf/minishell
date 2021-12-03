/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:48:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/02 20:28:37 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minis.h"
#include <stdio.h>
#include "parser.h"
#include "struct.h"

int		b_tree_add_child(t_node **node, t_bin *root);

//recursive with b_tree_add_child();
t_bin	*b_tree_init(t_node **node, int depth)
{
	t_bin	*root;

	root = ft_calloc(1, sizeof(t_bin));
	root->depth = depth;
	root->child_amount = count_children(*node);
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
		ret = 0;
		printf("handling token: %i and string: %s\n", ((t_token *)(*node)->content)->type, ((t_token *)(*node)->content)->string);
		if (((t_token *)(*node)->content)->type == CPAR)
			*node = (*node)->next;
		root->child[i] = add_com(node, root);//determines stage and fills comms
		if (root->child[i] == NULL)
		{
			*node = (*node)->next;
			root->child[i] = b_tree_init(node, root->depth + 1);
		}
		i++;
	}
	return (ret);
}
