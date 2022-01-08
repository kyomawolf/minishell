/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:05:19 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/07 23:14:15 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "exec.h"
#include "libft.h"
#include "lexer.h"
#include "minis.h"
#include <stdlib.h>

static void	ft_exchange_str_arr_sec(char ***str_arr, t_node *head)
{
	t_node	*temp;
	int		i;

	i = 0;
	while (head != NULL)
	{
		str_arr[0][i] = ((t_token *)head->content)->string;
		free((t_token *)head->content);
		head->content = NULL;
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
		i++;
	}
	str_arr[0][i] = NULL;
}

static void	ft_exchange_str_arr(char ***str_arr, t_node *head)
{
	int		len;

	len = ft_t_node_size(head);
	ft_free_char_array(*str_arr);
	*str_arr = NULL;
	if (head == NULL)
		return ;
	*str_arr = ft_calloc(len + 1, sizeof(char *));
	if (*str_arr == NULL)
		return ;
	ft_exchange_str_arr_sec(str_arr, head);
}

void	convert_str_arr_to_t_node_second(char **sel_dir, t_node *curr)
{
	int	i;

	i = 1;
	while (sel_dir[i] != NULL)
	{
		curr->next = ft_calloc(1, sizeof(t_node));
		if (curr->next == NULL)
		{
			free_t_node_list(curr);
			return ;
		}
		((t_node *)curr->next)->prev = curr;
		curr = curr->next;
		curr->content = ft_calloc(1, sizeof(t_token));
		if (curr->content == NULL)
		{
			free_t_node_list(curr);
			return ;
		}
		((t_token *)curr->content)->string = ft_strdup(sel_dir[i]);
		((t_token *)curr->content)->type = WORD;
		i++;
	}
}

static t_node	*convert_str_arr_to_t_node(char **sel_dir)
{
	t_node	*ret;
	t_node	*curr;

	curr = ft_calloc(1, sizeof(t_node));
	if (curr == NULL)
		return (NULL);
	curr->content = ft_calloc(1, sizeof(t_token));
	if (sel_dir[0] == NULL || curr->content == NULL)
	{
		free(curr);
		return (NULL);
	}
	((t_token *)curr->content)->string = ft_strdup(sel_dir[0]);
	if (((t_token *)curr->content)->string == NULL)
	{
		free_t_node_list(curr);
		return (NULL);
	}
	((t_token *)curr->content)->type = WORD;
	ret = curr;
	convert_str_arr_to_t_node_second(sel_dir, curr);
	return (ret);
}

int	ft_str_array_var_expansion(char ***str_arr, t_data *data)
{
	t_node	*head_token;

	if (*str_arr != NULL)
	{
		head_token = NULL;
		head_token = convert_str_arr_to_t_node(*str_arr);
		if (head_token == NULL)
			return (0);
		if (ft_t_token_variable_expansion(&head_token, data))
			return (1);
		if (ft_wildcard_expansion(&head_token))
			return (1);
		ft_exchange_str_arr(str_arr, head_token);
		if (str_arr == NULL)
			return (1);
	}
	return (0);
}
