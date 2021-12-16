/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:45:35 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/16 01:05:18 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minis.h"
#include "struct.h"
#include <stdio.h>

void	free_char_array(char ***arr)
{
	int	i;

	i = 0;
	if (*arr == NULL)
		return ;
	while (arr[0][i] != NULL)
	{
		free(arr[0][i]);
		arr[0][i] = NULL;
		i++;
	}
	free(*arr);
	*arr = NULL;
}

void	free_t_node_content_list(t_node *head)
{
	t_node	*temp;

	temp = NULL;
	while (head != NULL)
	{
		if (head->content != NULL)
		{
			free(head->content);
		}
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
}

void	free_token(t_token *token)
{
	if (token->string != NULL)
		free(token->string);
	token->string = NULL;
	while (token->heredoc != NULL)
	{
		free (token->heredoc->content);
		token->heredoc->content = NULL;
		temp = token->heredoc;
		token->heredoc = token->heredoc->next;
		free (temp);
		temp = NULL;
	}
	free(token);
	token = NULL;
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
		if (token != NULL)
		{
			free_token(token);
		}
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
}

void	free_main(t_data *data)
{
	if (data->input != NULL)
		free(data->input);
	if (data->prompt != NULL)
		free(data->prompt);
	if (data->input != NULL)
		free(data->currdir);
	free_t_node_content_list(data->envp);
	free(data);
}
