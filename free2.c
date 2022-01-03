/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:02:27 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/03 21:03:21 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minis.h"
#include "struct.h"
#include <stdio.h>

void	free_io_redir(t_bin *tree)
{
	t_node_io	*temp2;

	while (tree->io->redir != NULL)
	{
		if (tree->io->redir->file != NULL)
		{
			free(tree->io->redir->file);
			tree->io->redir->file = NULL;
		}
		temp2 = tree->io->redir;
		tree->io->redir = tree->io->redir->next;
		free(temp2);
		temp2 = NULL;
	}
}

void	free_io(t_bin *tree)
{
	if (tree->io == NULL)
		return ;
	if (tree->io->heredoc_node != NULL)
		free_t_node_content_list(tree->io->heredoc_node);
	if (tree->io->redir != NULL)
		free_io_redir(tree);
	if (tree->io->infile != NULL)
	{
		free(tree->io->infile);
		tree->io->infile = NULL;
	}
	free(tree->io);
	tree->io = NULL;
}

void	free_simplecommand(t_simple_com *command)
{
	if (command == NULL)
		return ;
	if (command->arguments != NULL)
		free_char_array(&(command->arguments));
	free(command);
	command = NULL;
}

void	free_tree(t_bin *tree)
{
	int	i;

	i = 0;
	if (tree == NULL)
		return ;
	while (tree->child != NULL && tree->child[i] != NULL)
	{
		free_tree(tree->child[i]);
		tree->child[i] = NULL;
		i++;
	}
	if (tree->child != NULL)
	{
		free(tree->child);
		tree->child = NULL;
	}
	if (tree->command != NULL)
		free_simplecommand(tree->command);
	if (tree->io != NULL)
		free_io(tree);
	free(tree);
	tree = NULL;
}
