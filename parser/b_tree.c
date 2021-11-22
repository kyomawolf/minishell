/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:48:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/22 18:11:10 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minis.h"
#include "parser.h"
#include "struct.h"

//recursive with b_tree_add_child();
t_bin	*b_tree_init(t_node *node, int depth)
{
	t_bin	*root;
	int		det_com;

	root = ft_calloc(1, sizeof(t_bin));
	root->child_amount = count_children(node);
	root->child = ft_calloc(root->child_amount + 1, sizeof(t_bin *));
	while (1)
	{
		det_com = par_check_com(node); //check for new word/op
		if (det_com == -1)
			break ;
		if (b_tree_add_child(node, root))
			break ;
	}
	return (root);
}

//recursive with b_tree_init();
t_bin	*b_tree_add_child(t_node *node, t_bin *root)
{
	int	i;
	int	ret;

	ret = 1;
	i = 0;
	while (root->child[i] != NULL)
		i++;
	while (i < root->child_pos)
	{
		ret = 0;
		root->child[i] = add_com(node->content);//determines stage and fills comms
		if (root->child[i] == NULL)
			root->child[i] = b_tree_init(node, , root->depth + 1);
		i++;
	}
	return (ret);
}
