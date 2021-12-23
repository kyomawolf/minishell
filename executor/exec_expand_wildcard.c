/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_wildcard.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 15:42:56 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/23 22:51:30 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "exec.h"
#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>

static int	ft_check_ambigious_redirection(t_node *head, t_node *list)
{
	t_token	*prev_token;

	if (((t_node *)head->prev) != NULL)
	{
		prev_token = ((t_token *)((t_node *)head->prev)->content);
		if (prev_token != NULL && (prev_token->type > HERE_DOC && \
			prev_token->type < QUOTE))
		{
			if (ft_t_node_size(list) > 1)
			{
				ft_t_node_free(list);
				printf("Error ambigious redirections\n");
				return (1);
			}
		}
	}
	return (0);
}

static int	ft_check_for_wc_expansion(t_node **head)
{
	t_token	*token;
	t_expand exp_data;
	int		i;

	token = (t_token *)(*head)->content;
	if (token->type == WORD)
	{
		i = 0;
		while (token->string[i] != '\0')
		{
			if (token->string[i] == '*')
			{
				exp_data.list = wild_main(token->string);
				if (exp_data.list == NULL)
					return (1);
				if (ft_check_ambigious_redirection(*head, exp_data.list))
					return (1);
				ft_exchange_tokens(head, &exp_data);
				break ;
			}
			i++;
		}
	}
	return (0);
}

int	ft_wildcard_expansion(t_node **head)
{
	t_node	*temp;

	temp = *head;
	while (*head != NULL)
	{
		if (ft_check_for_wc_expansion(head))
			return (1);
		temp = *head;
		*head = (*head)->next;
	}
	ft_get_beginning_of_list(temp, head);
	return (0);
}
