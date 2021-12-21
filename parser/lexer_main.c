/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:05:00 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 18:33:52 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
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
		printf("invalid token\n");
		ft_t_node_free(head);
		return (NULL);
	}
	else
	{
		if (ft_lexer_heredoc(head))
		{
			printf("syntax error near unexpected token: \\n\n");
			ft_t_node_free(head);
			return (NULL);
		}
	}
	return (head);
}
	//ft_s_node_print_content(head);
