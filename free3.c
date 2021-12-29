/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/29 18:58:57 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/29 20:11:56 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include <stdlib.h>

void	free_list_wo_content(t_node **head)
{
	t_node	*temp;

	while (*head != NULL)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp);
		temp = NULL;
	}
}

void	free_token2(t_node **node)
{
	void	*temp;

	temp = NULL;
	if (((t_token *)(*node)->content)->string != NULL)
		free(((t_token *)(*node)->content)->string);
	((t_token *)(*node)->content)->string = NULL;
	while (((t_token *)(*node)->content)->heredoc != NULL)
	{
		if (((t_token *)(*node)->content)->heredoc->content != NULL)
			free (((t_token *)(*node)->content)->heredoc->content);
		((t_token *)(*node)->content)->heredoc->content = NULL;
		temp = ((t_token *)(*node)->content)->heredoc;
		((t_token *)(*node)->content)->heredoc = \
		((t_token *)(*node)->content)->heredoc->next;
		free (temp);
		temp = NULL;
	}
	free((t_token *)(*node)->content);
	(*node)->content = NULL;
}

void	free_t_node_list2(t_node **head)
{
	t_node	*temp;

	temp = NULL;
	while (*head != NULL)
	{
		if ((t_token *)(*head)->content)
		{
			free_token2(head);
			(*head)->content = NULL;
		}
		temp = *head;
		*head = (*head)->next;
		free(temp);
		temp = NULL;
	}
}
