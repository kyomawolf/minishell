/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:54:36 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/23 16:35:47 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "exec.h"
#include "lexer.h"
#include <stdio.h>

void	traverse_tree_rec(t_bin *tree, t_node **head)
{
	int			i;
	static int	depth = 0;

	if (tree->child == NULL)
	{
		if (tree->io != NULL \
			|| (tree->command != NULL && tree->command->arguments != NULL))
		{
			tree->depth = depth;
			ft_t_node_add_back(head, ft_t_node_create(tree));
		}
		return ;
	}
	depth++;
	i = 0;
	while (tree->child[i] != NULL)
	{
		traverse_tree_rec(tree->child[i], head);
		i++;
	}
	depth--;
}

void	ft_free_pipeline(t_node **pipeline)
{
	t_node	*temp;

	while (*pipeline != NULL)
	{
		temp = *pipeline;
		*pipeline = (*pipeline)->next;
		free(temp);
		temp = NULL;
	}
}

static t_node	*create_pipeline(t_node *start, t_node *end)
{
	t_node	*pipeline;

	pipeline = NULL;
	while (1)
	{
		ft_t_node_add_back(&pipeline, ft_t_node_create(start->content));
		if (start == end)
			break ;
		start = start->next;
	}
	return (pipeline);
}

//DEBUG ONLY ----> delete later on
void	print_pipeline(t_node *pipeline)
{
	char	**cmd_arr;
	int		i;

	while (pipeline != NULL)
	{
		if (pipeline->content != NULL)
		{
			cmd_arr = ((t_bin *)pipeline->content)->command->arguments;
			i = 0;
			while (cmd_arr[i] != NULL)
			{
				printf("cmd_arr[%d] :%s: depth %d\n", i, cmd_arr[i], \
				((t_bin *)pipeline->content)->depth);
				i++;
			}
		}
		else
			printf("content NULL. go to next command\n");
		pipeline = pipeline->next;
	}
	printf("next pipeline\n");
}

t_node	*create_execution_pipeline(t_node **head, t_data *data)
{
	t_node	*start;
	t_node	*end;
	t_node	*temp;

	start = *head;
	while (*head != NULL)
	{
		temp = *head;
		if (ft_t_bin_variable_expansion(temp, data))
			return (NULL);
		if ((*head)->next == NULL || ((t_bin *)((t_node *)(*head)->next) \
			->content)->control_op != PIPE)
			break ;
		*head = (*head)->next;
	}
	end = *head;
	return (create_pipeline(start, end));
}
