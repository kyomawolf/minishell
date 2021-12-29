/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 16:45:22 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/29 20:10:02 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>

void	*ft_s_token_create(void *content)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->string = content;
	token->args = NULL;
	token->cmd_arr = NULL;
	token->heredoc = NULL;
	token->type = -1;
	token->quote_status = -1;
	return (token);
}

void	*ft_s_node_create(void *content)
{
	struct s_node	*node;

	node = (struct s_node *)malloc(sizeof(struct s_node));
	if (node == NULL)
		return (NULL);
	node->next = NULL;
	node->prev = NULL;
	node->content = content;
	return (node);
}

void	*ft_s_node_get_last(void *head)
{
	struct s_node	*current;

	current = head;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	ft_s_node_add_back(struct s_node **head, struct s_node *node)
{
	struct s_node	*current;

	if (*head == NULL)
		*head = node;
	else
	{
		current = ft_s_node_get_last(*head);
		node->prev = current;
		current->next = node;
	}
}

void	ft_s_node_free(struct s_node *head)
{
	struct s_node	*temp;

	while (head != NULL)
	{
		free(((struct s_token *)head->content)->string);
		head->content = NULL;
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
}
