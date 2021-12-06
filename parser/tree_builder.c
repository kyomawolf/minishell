/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:35:59 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/06 17:14:36 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "minis.h"
#include "libft.h"
#include "parser.h"

int	count_words(t_node *node)
{
	int	counter;

	counter = 0;
	while (node != NULL && ((t_token *)node->content)->type == WORD)
	{
		counter++;
		node = node->next;
	}
	return (counter);
}

t_node	*add_io(t_bin *tree, t_node *node)
{
	if (tree->io == NULL)
		tree->io = ft_calloc(1, sizeof(t_io));
	if (((t_token *)node->content)->type == IRD)
	{
		tree->io->i_count++;
		if (tree->io->input == NULL)
			tree->io->input = ft_calloc(1, sizeof(char *));
		else
			tree->io->input = ft_realloc_charpp(&tree->io->input, \
			ft_char_arr_len(tree->io->input) + 2);
		tree->io->input[ft_char_arr_len(tree->io->input)] = \
		ft_strdup(((t_token *)((t_node *)node->next)->content)->string);
	}
	else if (((t_token *)node->content)->type == HERE_DOC)
	{
		tree->io->heredoc_node = ((t_token *)node->content)->heredoc;
		((t_token *)node->content)->heredoc = NULL;
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
		ft_strdup(((t_token *)((t_node *)node->next)->content)->string);
	}
	printf("returning?!\n");
	return (((t_node *)node->next)->next);
}

t_node	*add_words(t_simple_com *command, t_node *node)
{
	int	n_words;
	int	i;

	n_words = count_words(node);
	printf("%i\n", n_words);
	if (command->number_arguments == 0)
		command->arguments = calloc(n_words + 1, sizeof(char *));
	else
		command->arguments = ft_realloc_charpp(&command->arguments, \
		command->number_arguments + n_words + 1);
	i = 0;
	while (i < n_words)
	{
		command->arguments[command->number_arguments + i] = ft_strdup(((t_token *)node->content)->string);
		node = node->next;
		i++;
	}
	command->number_arguments += n_words;
	return (node);
}

t_e_op	add_last_operator(t_node *node)
{
	while (node->prev != NULL)
	{
		if (((t_token *)node->content)->type < OPAR)
			return (((t_token *)node->content)->type);
		node = node->prev;
	}
	return (0);
}

t_bin	*add_node(t_bin *par, t_node *node)
{
	t_bin	*ret;

	ret = ft_calloc(1, sizeof(t_bin));
	ret->command = ft_calloc(1, sizeof(t_simple_com));
	ret->parent = par;
	ret->control_op = add_last_operator(node);
	return (ret);
}

t_bin	*add_com(t_node **ori_node, t_bin *parent)
{
	t_bin	*ret;
	t_e_op	token;

	token = ((t_token *)(*ori_node)->content)->type;
	if (token == OPAR)
		return (NULL);
	ret = add_node(parent, *ori_node);
	if (token < OPAR)
	{
		*ori_node = (*ori_node)->next;
		token = ((t_token *)(*ori_node)->content)->type;
	}
	while (*ori_node != NULL)
	{
		if (token > CPAR && token < QUOTE)
			*ori_node = add_io(ret, *ori_node);
		if (token < HERE_DOC)
			break ;
		*ori_node = add_words(ret->command, *ori_node);
		if (*ori_node != NULL)
			token = ((t_token *)(*ori_node)->content)->type;
	}
	printf("op: %i\n", ret->control_op);
	return (ret);
}

t_node	*jump_pars(t_node *node)
{
	int	i;

	i = 0;
	while (node != NULL)
	{
		if (((t_token *)node->content)->type == OPAR)
			i++;
		else if (((t_token *)node->content)->type == CPAR)
			i--;
		else if (i == 0)
			return (node);
		node = node->next;
	}
	return (node);
}

int	count_children(t_node *node)
{
	int		count;
	t_e_op	token;

	count = 0;
	token = ((t_token *)node->content)->type;
	if (token == WORD)
		count++;
	if (token == OPAR)
		node = node->next;
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
			node = jump_pars(node);
		}
		else if (token > CPAR && token < QUOTE)
			node = node->next;
		else
			node = node->next;
	}
	return (count);
}

t_bin	*builder_main(t_node *head)
{
	t_bin	*tree;
	t_node	*cpy;

	if (check_input(head))
	{
		write(1, "\nERROR\n", 7);
		return (NULL);
	}
	//return (NULL);
	cpy = head;
	tree = b_tree_init(&cpy, 0);
	print_binary_tree(tree, 3);
	printf("pipes: %i\n", pipe_count_main(tree));
	free_t_node_list(head);
	return (tree);
}

//int main(int argc, char **argv)
//{
//	t_node	*node;
//	t_bin	*tree;
//
//	node = wild_main(argv[1]);
//	tree = builder_main(node);
//}
