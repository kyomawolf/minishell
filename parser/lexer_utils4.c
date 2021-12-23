/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:59:58 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/23 22:48:19 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include <stdio.h>

//prints the string of a t_token struct (as content of a s_node list element)
// if the member heredoc is given, it prints its content aswell
void	ft_s_node_print_content(t_node *head)
{
	t_token	*token;
	t_node	*temp;

	while (head != NULL)
	{
		token = (t_token *)head->content;
		if (token->string != NULL)
			printf("[%d][%c]:%s:\n"\
			, token->type, token->quote_status, token->string);
		if (token->heredoc != NULL && token->heredoc->content != NULL)
		{
			temp = token->heredoc;
			while (temp != NULL)
			{
				printf(":%s:\n",(char *)temp->content);
				temp = temp->next;
			}
		}
		head = head->next;
	}
}

// USED ?
// skips set of chars, gives information if characters were skipped
int	ft_skip_chars(char *input, int *pos, char *chars)
{
	int	ret;
	int	temp;

	ret = 0;
	temp = *pos;
	while (ft_strchr(chars, input[*pos]) && input[*pos] != '\0')
		(*pos)++;
	if (temp != *pos)
		ret = 1;
	return (ret);
}

//checks if token has type of a redirection
int	ft_is_type_redirection(t_node *node)
{
	t_token	*token;
	int		ret;

	ret = 0;
	token = (t_token *)node->content;
	if (token->type == HERE_DOC)
		ret = 1;
	else if (token->type == IRD)
		ret = 1;
	else if (token->type == ORD_APP)
		ret = 1;
	else if (token->type == ORD_TRC)
		ret = 1;
	return (ret);
}
