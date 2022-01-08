/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 15:53:53 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/08 00:22:36 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include <stdlib.h>

//detaches t_node and frees the content of t_token (string) and token itself
// can only be used if to_detach is in the middle of a list
void	ft_t_node_detach_and_free(t_node *to_detach)
{
	((t_node *)to_detach->prev)->next = to_detach->next;
	if ((t_node *)to_detach->next != NULL)
		((t_node *)to_detach->next)->prev = to_detach->prev;
	if (to_detach->content != NULL)
	{
		free(((t_token *)to_detach->content)->string);
		((t_token *)to_detach->content)->string = NULL;
		free(to_detach->content);
		to_detach->content = NULL;
	}
	to_detach->next = NULL;
	to_detach->prev = NULL;
	free(to_detach);
	to_detach = NULL;
}

//frees t_token->heredoc and its related pointer(content and listelement itself)
// t_token is untouched
void	ft_t_node_free_heredoc(t_token *token, t_node *temp)
{
	while (token->heredoc != NULL)
	{
		free (token->heredoc->content);
		token->heredoc->content = NULL;
		temp = token->heredoc;
		token->heredoc = token->heredoc->next;
		free (temp);
		temp = NULL;
	}
}

// frees t_node and its content t_token
// (with all its related pointers:
//  char *string & t_node *heredoc (calls ft_t_node_free_heredoc))
void	ft_t_node_free(t_node *head)
{
	t_token	*token;
	t_node	*temp;

	temp = NULL;
	token = NULL;
	while (head != NULL)
	{
		token = (t_token *)head->content;
		free(token->string);
		token->string = NULL;
		if (token->heredoc != NULL)
			ft_t_node_free_heredoc(token, temp);
		free(token);
		token = NULL;
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
}

void	ft_free_lexer(t_word *word, t_node **head, t_data *data)
{
	t_node	*temp;

	free(data->input);
	data->input = NULL;
	if (word->chars != NULL)
	{
		free(word->chars);
		word->chars = NULL;
	}
	while (*head != NULL)
	{
		if ((*head)->content != NULL)
		{
			if (((t_token *)(*head)->content)->string != NULL)
			{
				free(((t_token *)(*head)->content)->string);
				((t_token *)(*head)->content)->string = NULL;
			}
			free ((*head)->content);
			(*head)->content = NULL;
		}
		temp = *head;
		*head = (*head)->next;
		free (temp);
	}
}
