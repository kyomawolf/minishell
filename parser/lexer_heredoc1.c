/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_heredoc1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:57:36 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/08 15:34:50 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "lexer.h"
#include <unistd.h>
#include <errno.h>

void	ft_signals_heredoc(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

static int	ft_lexer_handle_heredoc(t_node *temp)
{
	t_token	*token;
	t_token	*delimiter;
	int		ret;

	token = ((t_token *)temp->content);
	if (token->type == HERE_DOC)
	{
		if ((t_token *)((t_node *)temp->next) == NULL)
			return (1);
		delimiter = ((t_token *)((t_node *)temp->next)->content);
		if (delimiter->type != WORD)
			return (1);
		else
		{
			ret = ft_lexer_handle_heredoc_input(token, delimiter);
			if (ret == 2)
				return (2);
			token->quote_status = delimiter->quote_status;
			ft_t_node_detach_and_free(temp->next);
		}
	}
	return (0);
}

//searches list for token with type heredoc.
// If next list elements content is of type word
// the function ft_lexer_handle_heredoc_input is called.
// The quoted status of the "delimiter" token
// is copied to the here_doc token
// and the delimiter token is removed from the list.
int	ft_lexer_heredoc(t_node **head)
{
	t_node	*temp;
	int		ret;

	temp = *head;
	while (temp != NULL)
	{
		ret = ft_lexer_handle_heredoc(temp);
		if (ret == 1)
			return (1);
		else if (ret == 2)
			return (2);
		temp = temp->next;
	}
	return (0);
}
