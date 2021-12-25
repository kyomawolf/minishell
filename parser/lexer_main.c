/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:05:00 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/24 18:06:16 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "lexer.h"
#include "libft.h"
#include <stdio.h>

// checks if operator has valid amount of characters
static int	ft_operator_is_valid(t_node *head)
{
	t_token	*token;
	int		ret;

	ret = 0;
	while (head != NULL)
	{
		token = (t_token *)head->content;
		if (token->type != WORD && ft_strlen(token->string) > 2)
		{
			ret = 1;
			break ;
		}
		head = head->next;
	}
	return (ret);
}

t_node	*ft_lexer(char *input)
{
	t_node	*head;

	head = ft_get_token_list(input);
	if (ft_operator_is_valid(head))
	{
		ft_putstr_fd("Error: invalid operator token.\n", 2);
		free_t_node_list2(&head);
		return (NULL);
	}
	else
	{
		if (ft_lexer_heredoc(&head))
		{
			ft_putstr_fd("Error: syntax error near unexpected token: \\n\n", 2);
			free_t_node_list2(&head);
			return (NULL);
		}
	}
	//ft_s_node_print_content(head);
	return (head);
}
