/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:05:00 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/09 18:50:28 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "lexer.h"
#include "exec.h"
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

// separates input string into list of tokens.
// checks for invalid operator tokens
// and syntax errors related to usage of here_docs.
// returns t_node *head, which is the head of a list with content t_token *
t_node	*ft_lexer(t_data *data)
{
	t_node	*head;
	int		ret;

	head = ft_get_token_list(data);
	if (ft_operator_is_valid(head))
	{
		ft_putstr_fd("minishell: invalid operator token\n", 2);
		free_t_node_list2(&head);
		return (NULL);
	}
	else
	{
		ret = ft_lexer_heredoc(&head);
		if (ret == 1)
			ft_putstr_fd("minishell: ", 2);
		if (ret == 1)
			ft_putstr_fd("syntax error near unexpected token \\n\n", 2);
		if (ret > 0)
		{
			change_env_exit_status(data, 1);
			free_t_node_list2(&head);
			return (NULL);
		}
	}
	return (head);
}
