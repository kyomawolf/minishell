/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:45:35 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/01 15:15:58 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minis.h"
#include "struct.h"

void	free_char_array(char ***arr)
{
	int	i;

	i = 0;
	while ((*arr)[i] != NULL)
	{
		free((*arr)[i]);
		i++;
	}
	free(*arr);
}

struct s_simple_com	**free_simple_com_list(struct s_simple_com ***tofree, int l)
{
	int	i;
	int	ii;

	i = 0;
	while (i < l && (*tofree)[i] != NULL)
	{
		ii = 0;
		while (ii < (*tofree)[i]->number_arguments)
		{
			free((*tofree)[i]->arguments[ii]);
			ii++;
		}
		free((*tofree)[i]->arguments);
		free((*tofree)[i]);
		i++;
	}
	free(*tofree);
	*tofree = NULL;
	return (*tofree);
}

void	free_t_node_list(t_node *head)
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
		free(token);
		token = NULL;
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
}
