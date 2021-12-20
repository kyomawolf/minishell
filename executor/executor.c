/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 17:25:23 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/19 13:53:08 by mstrantz         ###   ########.fr       */
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

	if (tree->child == NULL)
	{
		if (tree->io != NULL \
			|| (tree->command != NULL && tree->command->arguments != NULL))
		{
			if (*head != NULL && tree->control_op != PIPE)
				ft_t_node_add_back(head, ft_t_node_create(NULL));
			ft_t_node_add_back(head, ft_t_node_create(tree));
		}
		return ;
	}
	i = 0;
	while (tree->child[i] != NULL)
	{
		traverse_tree_rec(tree->child[i], head);
		i++;
	}
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
	while (start != end)
	{
		ft_t_node_add_back(&pipeline, ft_t_node_create(start->content));
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

void	executor(t_node *head, t_data *data, int es)
{
	t_node	*start;
	t_node	*end;
	t_node	*pipeline;

	start = head;
	while (head != NULL && head->content != NULL)
	{
		if (ft_t_bin_variable_expansion(head, data))
			return ;
		head = head->next;
	}
	end = head;
	pipeline = create_pipeline(start, end);
	if ((es != 0 && ((t_bin *)pipeline->content)->control_op == OR)
		|| (es == 0 && ((t_bin *)pipeline->content)->control_op == AND))
		es = ft_execute(pipeline, data);
	ft_free_pipeline(pipeline);
	change_env_exit_status(data, es);
	if (head != NULL)
	{
		head = head->next;
		executor(head, data, es);
	}
}
