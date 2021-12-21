/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 15:52:43 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 19:56:35 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include <stdlib.h>

//creates and initializes t_token *
void	*ft_t_token_create(void *content)
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

//creates and initializes t_node *
void	*ft_t_node_create(void *content)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (node == NULL)
		return (NULL);
	node->next = NULL;
	node->prev = NULL;
	node->content = content;
	return (node);
}

// gets last not NULL element in t_node list
void	*ft_t_node_get_last(void *head)
{
	if (head != NULL)
	{
		while (((t_node *)head)->next != NULL)
			head = ((t_node *)head)->next;
	}
	return (head);
}

// adds t_node *node at the end of the list beginning with head
void	ft_t_node_add_back(t_node **head, t_node *node)
{
	t_node	*last;

	if (*head == NULL)
		*head = node;
	else
	{
		last = ft_t_node_get_last(*head);
		node->prev = last;
		last->next = node;
	}
}

// returns the amount of nodes in the list
int	ft_t_node_size(t_node *head)
{
	int	len;

	len = 0;
	while (head != NULL)
	{
		head = head->next;
		len++;
	}
	return (len);
}
