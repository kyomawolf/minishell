/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/02 15:31:22 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/03 19:45:14 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minis.h"
#include "libft.h"
#include "parser.h"
#include "struct.h"
#include <stdio.h>

t_node	*add_io_heredoc(t_bin *tree, t_node *node)
{
	if (tree->io->heredoc_node != NULL)
		free_t_node_content_list(tree->io->heredoc_node);
	tree->io->heredoc_node = ((t_token *)node->content)->heredoc;
	((t_token *)node->content)->heredoc = NULL;
	tree->io->quoted_status = ((t_token *)node->content)->quote_status;
	if (tree->io->infile)
		free(tree->io->infile);
	tree->io->infile = NULL;
	if (ft_t_node_io_append(tree, node, HERE_DOC))
		return ((void *)1);
	return ((t_node *)node->next);
}
