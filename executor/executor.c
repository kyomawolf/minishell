/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 17:25:23 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/20 21:59:18 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "minis.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void	*ft_t_node_create(void *content);
void	*ft_t_node_get_last(void *head);
void	ft_t_node_add_back(t_node **head, t_node *node);
void	ft_t_node_free(t_node *head);

void	traverse_tree_rec(t_bin *tree, t_node **head)
{
	int	i;
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

void	ft_free_pipeline(t_node *pipeline)
{
	t_node	*temp;

	while (pipeline != NULL)
	{
		temp = pipeline;
		pipeline = pipeline->next;
		free(temp);
		temp = NULL;
	}
}

t_node	*create_pipeline(t_node *start, t_node *end)
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

void	change_env_exit_status(t_data *data, int es)
{
	char	*status;
	t_node	*temp;

	temp = data->envp;
	while (temp != NULL)
	{
		if (ft_strnstr(temp->content, "?=\0", ft_strlen(temp->content)))
		{
			free (temp->content);
			status = ft_itoa(es);
			temp->content = ft_strjoin("?=", status);
			free (status);
			status = NULL;
			break ;
		}
		temp = temp->next;
	}
}

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
				printf("cmd_arr[%d] :%s: depth %d\n", i, cmd_arr[i], ((t_bin *)pipeline->content)->depth);
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

	start = *head;
	while (*head != NULL)
	{
		if (ft_t_bin_variable_expansion(*head, data))
			return NULL;
		if ((*head)->next == NULL || ((t_bin *)((t_node *)(*head)->next)\
			->content)->control_op != PIPE)
			break ;
		*head = (*head)->next;
	}
	end = *head;
	return (create_pipeline(start, end));
}

int	executor_rec(t_node *head, t_data *data, int es, t_node *start)
{
	int	last_depth;

	if (head != NULL)
	{
		if (start == head)
			head = head->next;
		last_depth = ((t_bin *)((t_node *)head->prev)->content)->depth;
		if (head == NULL)
			return (1);
		if (((t_bin *)head->content)->control_op == OR && es == 0)
		{
			while (head != NULL && ((t_bin *)head->content)->depth > last_depth)
				head = head->next;
		}
		executor(head, data, es);
	}
	(void)last_depth;
	return (0);
}

void	executor(t_node *head, t_data *data, int es)
{
	t_node	*start;
	t_node	*pipeline;

	start = head;
	pipeline = create_execution_pipeline(&head, data);
	if (pipeline == NULL)
		return ;
	if ((es != 0 && ((t_bin *)pipeline->content)->control_op == OR)
		|| (es == 0 && ((t_bin *)pipeline->content)->control_op == AND))
		es = ft_execute(pipeline, data);
	ft_free_pipeline(pipeline);
	change_env_exit_status(data, es);
	if (executor_rec(head, data, es, start))
		return ;
}
