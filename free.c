/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:45:35 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/15 19:55:23 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minis.h"
#include "struct.h"
#include <stdio.h>

void	free_char_array(char ***arr)
{
	int	i;

	i = 0;
	if (*arr == NULL)
		return ;
	while ((*arr)[i] != NULL)
	{
		free((*arr)[i]);
		i++;
	}
	free(*arr);
}

void	free_t_node_content_list(t_node *head)
{
	t_node	*temp;

	temp = NULL;
	while (head != NULL)
	{
		if (head->content != NULL)
		{
			free(head->content);
		}
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
}

void	free_t_node_list(t_node *head)
{
	t_token	*token;
	t_node	*temp;

	temp = NULL;
	token = NULL;
	while (head != NULL)
	{
		token = (t_token *)head->content;
		if (token != NULL)
		{
			if (token->string != NULL)
				free(token->string);
			token->string = NULL;
			while (token->heredoc != NULL)
			{
				free (token->heredoc->content);
				token->heredoc->content = NULL;
				temp = token->heredoc;
				token->heredoc = token->heredoc->next;
				free (temp);
				temp = NULL;
			}
			free(token);
			token = NULL;
		}
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
}

void	free_io(t_io *io)
{
	if (io == NULL)
		return ;
	free_t_node_content_list(io->heredoc_node);
	if (io->input != NULL)
		free_char_array(&(io->input));
	if (io->output != NULL)
		free_char_array(&(io->output));
	free(io);
}

void	free_simplecommand(t_simple_com *command)
{
	if (command == NULL)
		return ;
	if (command->arguments != NULL)
		free_char_array(&(command->arguments));
	free(command);
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
		i++;
	}
	if (tree->child != NULL)
		free(tree->child);
	if (tree->command != NULL)
		free_simplecommand(tree->command);
	if (tree->io != NULL)
		free_io(tree->io);
	free(tree);
	tree = NULL;
}

void	free_main(t_data *data)
{
	if (data->input != NULL)
		free(data->input);
	if (data->prompt != NULL)
		free(data->prompt);
	if (data->input != NULL)
		free(data->currdir);
	free_t_node_content_list(data->envp);
	free(data);
}
