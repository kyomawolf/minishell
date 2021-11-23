/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:35:59 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/23 23:13:37 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "minis.h"
#include "parser.h"

int	count_words(t_node *node)
{
	int	counter;

	counter = 0;
	while (node == NULL && node->content->type == WORD)
		counter++;
	return (counter);
}

node	*add_io(t_bin *tree, t_node *node)
{
	if (tree->io == NULL)
		tree->io = ft_calloc(1, sizeof(t_io));
	if (node->content->type == HERE_DOC || node->content->type == IRD)
	{
		tree->io->i_mode = node->content->type - HERE_DOC + 1;
		if (tree->io->input == NULL)
			tree->io->input = ft_calloc(1, sizeof(char *));
		else
			tree->io->input = ft_realloc_charpp(&tree->io->input, \
			ft_char_arr_len(tree->io->input) + 2);
		tree->io->input[ft_char_arr_len(tree->io->input)] = \
		ft_strdup(node->next->content->string);
	}
	else
	{
		tree->io->o_mode = node->content->type - HERE_DOC + 1;
		if (tree->io->output == NULL)
			tree->io->output = ft_calloc(1, sizeof(char *));
		else
			tree->io->output = ft_realloc_charpp(&tree->io->output, \
			ft_char_arr_len(tree->io->output) + 2);
		tree->io->output[ft_char_arr_len(tree->io->output)] = \
		ft_strdup(node->next->content->string);
	}
	return (node->next->next);
}

t_node	*add_words(t_simple_com *command, t_node *node)
{
	int	n_words;
	int	i;

	n_words = count_words(node);
	command->number_arguments += n_words;
	if (command->number_arguments == n_words)
		command->arguments = calloc(n_words + 1, sizeof(char *));
	else
		command->arguments = ft_realloc_charpp(&command->arguments, \
		command->number_arguments);
	i = command->number_arguments - n_words;
	while (i < n_words)
	{
		command->arguments[i] = ft_strdup(node->content->string);
		node = node->next;
		i++;
	}
	return (node);
}

t_bin	*add_operator(t_e_op en_op, t_bin *parent)
{
	t_bin	*ret;

	ret = ft_calloc(1, sizeof(t_bin));
	ret->parent = parent;
	ret->e_op = en_op;
	ret->command = ft_calloc(1, sizeof(t_simple_com));
	ret->child = NULL;
	return (ret);
}

t_bin	*add_com(t_node **ori_node, t_bin *parent)
{
	t_bin	*ret;
	t_node	*node;
	t_e_op	token;

	node = *ori_node;
	token = node->content->type;
	if (token == OPAR)
		return (NULL);
	if (token < OPAR)
	{
		ret = add_operator(token, parent);
		node = node->next;
		token = node->content->type;
	}
	while (node != NULL)
	{
		if (token > CPAR && token < QUOTE)
			node = add_io(ret, node);
		if (token < OPAR)
			break ;
		node = add_words(ret->command, node);
		node = node->next;
		node->content->type;
	}
	return (ret);
}

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
