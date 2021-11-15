/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:48:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/15 15:25:21 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minis.h"
#include "parser.h"
#include "struct.h"

//recursive with b_tree_add_child();
t_bin	*b_tree_init(t_data *data, int *c_pos, int depth)
{
	t_bin	*root;
	int		det_com;
	int		old_pos;

	old_pos = *c_pos;
	root = ft_calloc(1, sizeof(t_bin));
	root->child_amount = count_depth(data, old_pos);
	root->child = ft_calloc(root->child_amount + 1, sizeof(t_bin *));
	while (1)
	{
		det_com = par_check_com(data, c_pos);
		if (det_com == -1)
			break ;
		if (b_tree_add_child(data, old_pos, root, c_pos))
			break ;
	}
	return (root);
}

//recursive with b_tree_init();
t_bin	*b_tree_add_child(t_data *data, int	old_pos, t_bin *root, int *c_pos)
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
		root->child[i] = add_com(data, c_pos]);
		if (root->child[i] == NULL)
			root->child[i] = b_tree_init(data, c_pos, root->depth + 1);
		i++;
	}
	return (ret);
}
