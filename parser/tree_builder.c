/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:35:59 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/22 18:11:25 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "minis.h"
#include "parser.h"

t_bin	*add_com(t_token *tok);
int		par_check_com(t_node *node);
int		count_children(t_node *node);


t_bin	*builder_main(t_node *head)
{
	t_bin	*tree;

	tree = b_tree_init(head, 0);
	free_t_node_list(head);
	return (tree);
}
