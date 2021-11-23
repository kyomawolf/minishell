/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:35:59 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/23 19:24:16 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "minis.h"
#include "parser.h"

t_bin	*add_com(t_token *tok);
int		par_check_com(t_node *node);

t_node	*jump_token(t_node *node, int cmp)
{
	while (node != NULL)
	{
		if (node->content->type == cmp)
			return (node->next);
		node = node->next;
	}
	return (node);
}

int	count_children(t_node *node)
{
	int		count;
	int		sw;
	t_e_op	token;

	count = 1;
	while (node != NULL)
	{
		token = node->content->type;
		if (token < OPAR)
			count++;
		if (token == CPAR)
			break ;
		if (token == OPAR)
		{
			count++;
			node = jump_token(node, 4);
		}
		else if (node > CPAR && node < QUOTE)
			count++;
		else
			node = node->next;
	}
	return (count);
}

int	check_pars(t_node *head)
{
	int	pars;
	int	token;
	int	last_token;

	par = 0;
	last_token = -1;
	while (head != NULL)
	{
		token = head->content->type;
		if (token == CPAR && last_token == OPAR)
			return (1);
		if (token == CPAR)
			par--;
		if (token == OPAR)
			par++;
		if (par < 0)
			return (1);
		last_token = token;
		head = head->next;
	}
	if (par != 0)
		return (1);
	return (0);
}

int	check_io(t_node *head)
{
	int	sw;

	sw = 0;
	while (head != NULL)
	{
		token = head->content->type;
		if (token > HERE_DOC && token < QUOTE && !sw)
			sw += 1;
		else if (token == WORD)
			sw -= 1;
		else
			return (1);
		head = head->next;
	}
	if (sw != 0)
		return (1);
	return (0);
}

int	check_op(t_node *head)
{
	int	sw;

	sw = -1;
	while (head != NULL)
	{
		token = head->content->type;
		if (token < OPAR && (sw == 1 || sw == 2))
			return (1);
		if (token < OPAR && sw == -1)
			return (1);
		if (token < OPAR && sw == 0)
			sw = 1;
		else if (token == CPAR || token == WORD)
			sw = 0;
		else
			sw = 2;
		head = head->next;
	}
	if (sw == -1 || sw == 1)
		return (1);
	return (0);
}

int	check_input(t_node *head)
{
	int	err;

	err = check_pars(head);
	if (err != -1)
		return (err);
	err = check_io(head);
	if (err != -1)
		return (err);
	err = check_op(head);
	return (err);
}

t_bin	*builder_main(t_node *head)
{
	t_bin	*tree;

	if (check_input(head) != -1)
		return (NULL);
	tree = b_tree_init(head, 0);
	free_t_node_list(head);
	return (tree);
}
