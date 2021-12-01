/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:35:59 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/01 15:27:11 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "structs.h"
#include "minis.h"
#include "libft.h"
#include "parser.h"

int	count_words(t_node *node)
{
	int	counter;

	counter = 0;
	while (node == NULL && ((t_token *)node->content)->type == WORD)
		counter++;
	return (counter);
}

t_node	*add_io(t_bin *tree, t_node *node)
{
	if (tree->io == NULL)
		tree->io = ft_calloc(1, sizeof(t_io));
	if (((t_token *)node->content)->type == HERE_DOC || ((t_token *)node->content)->type == IRD)
	{
		tree->io->i_mode = ((t_token *)node->content)->type - HERE_DOC + 1;
		if (tree->io->input == NULL)
			tree->io->input = ft_calloc(1, sizeof(char *));
		else
			tree->io->input = ft_realloc_charpp(&tree->io->input, \
			ft_char_arr_len(tree->io->input) + 2);
		tree->io->input[ft_char_arr_len(tree->io->input)] = \
		ft_strdup(((t_token *)((t_node *)node->next)->content)->string);
	}
	else
	{
		tree->io->o_mode = ((t_token *)node->content)->type - HERE_DOC + 1;
		if (tree->io->output == NULL)
			tree->io->output = ft_calloc(1, sizeof(char *));
		else
			tree->io->output = ft_realloc_charpp(&tree->io->output, \
			ft_char_arr_len(tree->io->output) + 2);
		tree->io->output[ft_char_arr_len(tree->io->output)] = \
		ft_strdup(((t_token *)((t_node *)node->next)->content)->string);//HERE PLS DONT DO THIS ANYMORE
	}
	return (((t_node *)node->next)->next);
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
		command->arguments[i] = ft_strdup(((t_token *)node->content)->string);
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
	ret->control_op = en_op;
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
	token = ((t_token *)node->content)->type;
	if (token == OPAR)
		return (NULL);
	if (token < OPAR)
	{
		ret = add_operator(token, parent);
		node = node->next;
		token = ((t_token *)node->content)->type;
	}
	while (node != NULL)
	{
		if (token > CPAR && token < QUOTE)
			node = add_io(ret, node);
		if (token < OPAR)
			break ;
		node = add_words(ret->command, node);
		node = node->next;
		token = ((t_token *)node->content)->type;
	}
	return (ret);
}

t_node	*jump_token(t_node *node, int cmp)
{
	while (node != NULL)
	{
		if (((t_token *)node->content)->type == cmp)
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
		token = ((t_token *)node->content)->type;
		if (token < OPAR)
			count++;
		if (token == CPAR)
			break ;
		if (token == OPAR)
		{
			count++;
			node = jump_token(node, 4);
		}
		else if (token > CPAR && token < QUOTE)
			count++;
		else
			node = node->next;
	}
	return (count);
}

t_bin	*builder_main(t_node *head)
{
	t_bin	*tree;

	if (check_input(head) != -1)
	{
		write(1, "\nERROR\n", 7);
		return (NULL);
	}
	tree = b_tree_init(head, 0);
	free_t_node_list(head);
	return (tree);
}

int main(int argc, char **argv)
{
	t_node	*node;
	t_bin	*tree;

	node = wild_main(argv[1]);
	tree = builder_main(node);
	print_binary_tree(tree, 2);
}
