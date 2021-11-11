/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:48:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/11 17:05:59 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minis.h"
#include "parser.h"
#include "struct.h"



t_bin	*b_tree_init(t_data *data, int c_pos)
{
	t_bin	*root;
	int		node_count;
	t_bin	**all_children;

	node_count = ft_count_commands(data->input, c_pos, delimiter);
	root = ft_calloc(1, sizeof(t_bin));
	all_children = ft_calloc(node_count, sizeof(t_bin));
	
}

//recursive
t_bin	*b_tree_add_children(void)
{
	
}
