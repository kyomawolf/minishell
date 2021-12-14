/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_wildcard.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 15:42:56 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/14 22:53:13 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "../include/structs.h"
#include "../lib/libft/libft.h"

t_node	*wild_main(char *string);
void	ft_exchange_tokens(t_node **head, t_node *list);
void	ft_get_beginning_of_list(t_node *temp, t_node **head);
int		ft_t_node_size(t_node *head);
void	ft_t_node_free(t_node *head);

int	ft_check_ambigious_redirection(t_node *head, t_node *list)
{
	t_token	*prev_token;

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
	return (0);
}

int	ft_check_for_wc_expansion(t_node **head)
{
	t_token	*token;
	t_node	*list;
	int		i;

	token = (t_token *)(*head)->content;
	if (token->type == WORD)
	{
		i = 0;
		while (token->string[i] != '\0')
		{
			if (token->string[i] == '*')
			{
				list = wild_main(token->string);
				if (list == NULL)
					return (1);
				if (ft_check_ambigious_redirection(*head, list))
					return (1);
				ft_exchange_tokens(head, list);
				break ;
			}
			i++;
		}
	}
	return (0);
}

/* //sets *head to the beginning of the list
void	ft_get_beginning_of_list(t_node *temp, t_node **head)
{
	*head = temp;
	while ((*head)->prev != NULL)
		*head = (*head)->prev;
} */

int	ft_wildcard_expansion(t_node **head)
{
	t_node	*temp;

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
