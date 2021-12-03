/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_wildcard.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 15:42:56 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/03 17:56:52 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "../include/structs.h"
#include "../include/libft.h"

t_node	*wild_main(char *string);
void	ft_exchange_tokens(t_node **head, t_node *list);
void	ft_get_beginning_of_list(t_node *temp, t_node **head);

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
				printf("%d\n", __LINE__);
				list = wild_main(token->string);
				if (list == NULL)
					return (1);
				printf("%d\n", __LINE__);
				ft_exchange_tokens(head, list);
				printf("%d\n", __LINE__);
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
