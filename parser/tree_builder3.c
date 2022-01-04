/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:07:47 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/04 18:26:42 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minis.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>

int	ft_t_node_io_append(t_bin *tree, t_node *node, int type)
{
	t_node_io	*node_io;

	if (type == HERE_DOC)
		node_io = ft_t_node_io_create("__here_doc", HERE_DOC);
	else
	{
		node_io = ft_t_node_io_create(((t_token *)((t_node *)node->next) \
			->content)->string, type);
	}
	if (node_io == NULL)
		return (1);
	ft_t_node_io_add_back(&(tree->io->redir), node_io);
	if (type == IRD)
	{
		if (tree->io->infile)
			free(tree->io->infile);
		tree->io->infile = ft_strdup(node_io->file);
	}
	return (0);
}

int	add_io_i(t_bin *tree, t_node *node)
{
	tree->io->i_count++;
	if (ft_t_node_io_append(tree, node, IRD))
		return (1);
	return (0);
}

int	add_io_helper(t_bin *tree, t_node *node)
{
	tree->io->o_mode = ((t_token *)node->content)->type - HERE_DOC + 1;
	if (ft_t_node_io_append(tree, node, ((t_token *)node->content)->type))
		return (1);
	return (0);
}

t_node	*add_io(t_bin *tree, t_node *node)
{
	if (((t_token *)node->content)->type != HERE_DOC && node->next == NULL)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return ((void *) 1);
	}
	if (tree->io == NULL)
		tree->io = ft_calloc(1, sizeof(t_io));
	if (((t_token *)node->content)->type == IRD)
	{
		if (add_io_i(tree, node))
			return ((void *)1);
	}
	else if (((t_token *)node->content)->type == HERE_DOC)
		return (add_io_heredoc(tree, node));
	else
	{
		if (add_io_helper(tree, node))
			return ((void *)1);
	}
	return (((t_node *)node->next)->next);
}

t_node	*add_words(t_simple_com *command, t_node *node)
{
	int	n_words;
	int	i;

	n_words = count_words(node);
	if (n_words == 0)
		return (node);
	if (command->number_arguments == 0)
		command->arguments = calloc(n_words + 1, sizeof(char *));
	else
		command->arguments = ft_realloc_charpp(&command->arguments, \
		command->number_arguments + n_words + 1);
	i = 0;
	while (i < n_words)
	{
		command->arguments[command->number_arguments + i] = \
		ft_strdup(((t_token *)node->content)->string);
		node = node->next;
		i++;
	}
	command->number_arguments += n_words;
	return (node);
}
