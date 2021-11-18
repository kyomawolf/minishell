/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 16:45:22 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/17 17:17:11 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include "libft.h"

//void	*ft_s_node_insert(t_node **head, t_node *new, int index, t_node **point)
//{
//	int		i;
//	t_node	*temp;
//
//	i = 0;
//	temp = *head;
//	if (*point != NULL)
//		index = -1;
//	while (temp != *point && i < index)
//	{
//		temp = temp->next;
//		if (*point == NULL && i + 1 == index)
//			*point = temp;
//	}
//}

void	*ft_s_token_create(void *content)
{
	struct s_token	*token;

	token = (struct s_token *)malloc(sizeof(struct s_token));
	if (token == NULL)
		return (NULL);
	token->string = content;
	token->type = ft_s_token_get_type(content);
	token->layer = 0;
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