/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:57:36 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/24 18:00:11 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "lexer.h"
#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

//frees readline output if necessary and sets token->heredoc with input from
// heredoc
static void	ft_lexer_handle_heredoc_input_helper(t_token *token,
			t_node *head, char **line)
{
	if (*line != NULL)
	{
		free(*line);
		*line = NULL;
	}
	if (head != NULL)
	{
		token->heredoc = head;
	}
	return ;
}

// reads stdin, saves input in list until line is equal to delimiter string
// saves list of input in token member heredoc
static int	ft_lexer_handle_heredoc_input(t_token *token, t_token *delimiter)
{
	char	*line;
	size_t	len;
	t_node	*node;
	t_node	*head;

	head = NULL;
	node = NULL;
	len = ft_strlen(delimiter->string) + 1;
	while (1)
	{
		line = readline(">");
		if (line == NULL || !ft_strncmp(delimiter->string, line, len))
			break ;
		else
		{
			node = ft_t_node_create(line);
			if (node == NULL)
			{
				if (line != NULL)
					free(line);
				return (1);
			}
			ft_t_node_add_back(&head, node);
		}
	}
	ft_lexer_handle_heredoc_input_helper(token, head, &line);
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
	t_token	*token;
	t_token	*delimiter;
	t_node	*temp;

	temp = *head;
	while (temp != NULL)
	{
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
				ft_lexer_handle_heredoc_input(token, delimiter);
				token->quote_status = delimiter->quote_status;
				ft_t_node_detach_and_free(temp->next);
			}
		}
		temp = temp->next;
	}
	return (0);
}

/* int	ft_lexer_heredoc(t_node *head)
{
	t_token	*token;
	t_token	*delimiter;

	while (head != NULL)
	{
		token = ((t_token *)head->content);
		if (token->type == HERE_DOC)
		{
			if ((t_token *)((t_node *)head->next) == NULL)
				return (1);
			delimiter = ((t_token *)((t_node *)head->next)->content);
			if (delimiter->type != WORD)
				return (1);
			else
			{
				ft_lexer_handle_heredoc_input(token, delimiter);
				token->quote_status = delimiter->quote_status;
				ft_t_node_detach_and_free(head->next);
			}
		}
		head = head->next;
	}
	return (0);
} */
