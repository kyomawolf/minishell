/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:02:27 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 20:17:19 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minis.h"
#include "struct.h"
#include <stdio.h>

void	free_io(t_io *io)
{
	if (io == NULL)
		return ;
	if (io->heredoc_node != NULL)
		free_t_node_content_list(io->heredoc_node);
	if (io->input != NULL)
		free_char_array(&(io->input));
	if (io->output != NULL)
		free_char_array(&(io->output));
	free(io);
	io = NULL;
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
		free_io(tree->io);
	free(tree);
	tree = NULL;
}
