/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:07:47 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 16:53:45 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minis.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>

void	add_io_i(t_bin *tree, t_node *node)
{
	int	idx;

	tree->io->i_count++;
	if (tree->io->input == NULL)
		tree->io->input = ft_calloc(2, sizeof(char *));
	else
		tree->io->input = ft_realloc_charpp(&tree->io->input, \
		ft_char_arr_len(tree->io->input) + 2);
	idx = ft_char_arr_len(tree->io->input);
	tree->io->input[idx] = \
	ft_strdup(((t_token *)((t_node *)node->next)->content)->string);
	tree->io->infile = tree->io->input[idx];
}

t_node	*add_io(t_bin *tree, t_node *node)
{
	if (tree->io == NULL)
		tree->io = ft_calloc(1, sizeof(t_io));
	if (((t_token *)node->content)->type == IRD)
	{
		add_io_i(tree, node);
	}
	else if (((t_token *)node->content)->type == HERE_DOC)
	{
		tree->io->heredoc_node = ((t_token *)node->content)->heredoc;
		((t_token *)node->content)->heredoc = NULL;
		tree->io->quoted_status = ((t_token *)node->content)->quote_status;
		tree->io->infile = NULL;
		return ((t_node *)node->next);
	}
	else
	{
		tree->io->o_mode = ((t_token *)node->content)->type - HERE_DOC + 1;
		if (tree->io->output == NULL)
			tree->io->output = ft_calloc(2, sizeof(char *));
		else
			tree->io->output = ft_realloc_charpp(&tree->io->output, \
			ft_char_arr_len(tree->io->output) + 2);
		tree->io->output[ft_char_arr_len(tree->io->output)] = \
		ft_strdup(((t_token *)((t_node *)node->next)->content)->string);
		printf("%s\n", tree->io->output[0]);
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
